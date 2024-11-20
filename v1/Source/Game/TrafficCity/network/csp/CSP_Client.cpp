#include "CSP_Client.h"

#include "TrafficCity/util/copy_node.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/SmoothedTransform.h>
#include <algorithm>

CSP_Client::CSP_Client(Context * context) :
	Component(context)
{
	SubscribeToEvent(E_NETWORKMESSAGE, URHO3D_HANDLER(CSP_Client, HandleNetworkMessage));
}

void CSP_Client::RegisterObject(Context * context)
{
	context->RegisterFactory<CSP_Client>();
}

void CSP_Client::SetScene(Scene *scene) {
    scene_ = scene;
}

void CSP_Client::addInput(Controls& new_input)
{
	// Increment the update ID by 1
	++id;
	// Tag the new input with an id, so the id is passed to the server
	new_input.extraData_["id"] = id;
	// Add the new input to the input buffer
	inputBuffer.push_back(new_input);

	// Send to the server
	sendInput(new_input);

	//GetSubsystem<DebugHud>()->SetAppStats("client id: ", id);
	//GetSubsystem<DebugHud>()->SetAppStats("add_input() input_buffer.size(): ", input_buffer.size());
}

void CSP_Client::sendInput(Controls & controls)
{
	auto server_connection = GetSubsystem<Network>()->GetServerConnection();
	if (!server_connection ||
		!server_connection->GetScene() ||
		!server_connection->IsSceneLoaded())
		return;

	inputMessage.Clear();
	inputMessage.WriteUInt(controls.buttons_);
	inputMessage.WriteFloat(controls.yaw_);
	inputMessage.WriteFloat(controls.pitch_);
	inputMessage.WriteVariantMap(controls.extraData_);

	server_connection->SendMessage(MSG_CSP_INPUT, false, false, inputMessage);
	//server_connection->SendMessage(MSG_CSP_INPUT, true, true, input_message);

	GetSubsystem<DebugHud>()->SetAppStats("sent_inputs: ", ++sentInputs);
}

void CSP_Client::readLastId(MemoryBuffer & message)
{
	// Read last input ID
	auto new_server_id = message.ReadUInt();

	// Make sure it's more recent than the previous last ID since we're sending unordered messages
	// Handle range looping correctly
	if (id > serverId) {
		if (new_server_id < serverId)
			return;
	}
	else {
		if (new_server_id > serverId)
			return;
	}

	serverId = new_server_id;
	GetSubsystem<DebugHud>()->SetAppStats("serverId: ", serverId);
}


void CSP_Client::predict()
{
	//copy_scene();

	removeObsoleteHistory();
	reapplyInputs();
}


void CSP_Client::reapplyInputs()
{
	//auto scene = GetScene();
    auto scene = scene_;
    if (scene) {
        auto physicsWorld = scene->GetComponent<PhysicsWorld>();
        if (physicsWorld) {
            const auto timestep = 1.f / physicsWorld->GetFps();

            //GetSubsystem<DebugHud>()->SetAppStats("reapply_inputs() input_buffer.size(): ", input_buffer.size());

            // step a tick
            for (auto &controls: inputBuffer) {
                currentControls = &controls;
          //      physicsWorld->Update(timestep);
            }

            // current controls should only be used while predicting
            currentControls = nullptr;
        }
    }
}


void CSP_Client::removeObsoleteHistory()
{
	inputBuffer.erase(
		std::remove_if(inputBuffer.begin(), inputBuffer.end(), [&](Controls& controls) {
			unsigned update_id = controls.extraData_["id"].GetUInt();
			// Handle value range looping correctly
			if (id >= serverId)
			{
				if (update_id <= serverId ||
					update_id > id)
					return true;
			}
			else
			{
				if (update_id >= serverId ||
					update_id < id)
					return true;
			}
			return false;
		}),
		inputBuffer.end());
}


void CSP_Client::HandleNetworkMessage(StringHash eventType, VariantMap & eventData)
{
	auto network = GetSubsystem<Network>();

	using namespace NetworkMessage;
	const auto message_id = eventData[P_MESSAGEID].GetInt();
	auto connection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());
	MemoryBuffer message(eventData[P_DATA].GetBuffer());

	if (network->GetServerConnection())
	{
		switch (message_id)
		{
		case MSG_CSP_STATE:
			// read last input
			readLastId(message);

			if (enableCopy) {
				// read state snapshot
				auto scene = network->GetServerConnection()->GetScene();
				snapshot.readState(&message, scene);
				//scene->ApplyAttributes();
			}

			// Perform client side prediction
			predict();

			break;
		}
	}
}
