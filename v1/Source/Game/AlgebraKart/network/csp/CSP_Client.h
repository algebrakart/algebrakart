#pragma once

#include "CSP_messages.h"
#include "StateSnapshot.h"
#include <Urho3D/Scene/Component.h>
#include <vector>

namespace Urho3D
{
	class Context;
	class Controls;
	class Connection;
	class MemoryBuffer;
}

using namespace Urho3D;


/*
Client side prediction client.

- sends input to server
- receive state snapshot from server and run prediction
*/
struct CSP_Client : Component
{
	URHO3D_OBJECT(CSP_Client, Component);

	CSP_Client(Context* context);

	using ID = unsigned;

	// Register object factory and attributes.
	static void RegisterObject(Context* context);

	// Current's tick controls input
	Controls const* currentControls = nullptr;

	// Tags the input with "id" extraData, adds it to the input buffer
	void addInput(Controls& input);

	// do client-side prediction
	void predict();

	bool enableCopy = true; //used for testing

    virtual void SetScene(Scene *scene);


protected:
    WeakPtr<Scene> scene_;

    StateSnapshot snapshot;

	// current client-side update ID
	ID id = 0;
	// The current received ID from the server
	ID serverId = -1;

	// Input buffer
	std::vector<Controls> inputBuffer;

	// reusable buffer
	VectorBuffer inputMessage;


	// for debugging
	unsigned sentInputs = 0; // number of inputs sent


	// Handle custom network messages
	void HandleNetworkMessage(StringHash eventType, VariantMap& eventData);


	// read server's last received ID
	void readLastId(MemoryBuffer& message);

	// Sends the client's input to the server
	void sendInput(Controls& controls);

	// Re-apply all the inputs since after the current server ID to the current ID to correct the current network state.
	void reapplyInputs();

	// Remove all the elements in the buffer which are behind the server_id, including it since it was already applied.
	void removeObsoleteHistory();

};
