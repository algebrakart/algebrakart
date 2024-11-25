
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>

#include "Server.h"
#include "ClientObj.h"
#include "Vehicle.h"

#include <Urho3D/DebugNew.h>

class Vehicle;
//=============================================================================
//=============================================================================
Server::Server(Context* context)
    : Object(context)
{
    SubscribeToEvents();
}

Server::~Server()
{
}

void Server::RegisterScene(Scene *scene)
{
    scene_ = scene;
}

void Server::InitializeScene(File &file)
{
    scene_->InstantiateXML(file, Vector3::ZERO, Quaternion());
    File saveFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Scenes/" + "algebrakart-ServerSceneInit.xml", FILE_WRITE);
    scene_->SaveXML(saveFile);
    URHO3D_LOGINFOF("InitializeScene: Scene checksum -> %s", ToStringHex(scene_->GetChecksum()).CString());
}

bool Server::StartServer(unsigned short port)
{
    // Create a new recorder
    recorder_ = context_->CreateObject<Recorder>();
    recorder_->Reset(context_);
    return GetSubsystem<Network>()->StartServer(port);
}

/*
bool Server::Connect(const String &addressRequet, unsigned short port, const VariantMap& identity)
{
    Network* network = GetSubsystem<Network>();
    String address = addressRequet.Trimmed();

    if (address.Empty())
    {
        address = "localhost"; // Use localhost to connect if nothing else specified
    }

    // Connect to server, specify scene to use as a client for replication
    // Reset own object ID from possible previous connection
    networkActorNodeId_ = 0;
    vehicleNodeId_= 0;

    return network->Connect(address, port, scene_, identity);
}*/

void Server::Disconnect()
{
    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    // If we were connected to server, disconnect. Or if we were running a server, stop it. In both cases clear the
    // scene of all replicated content, but let the local nodes & components (the static world + camera) stay
    if (serverConnection)
    {
        serverConnection->Disconnect();
        scene_->Clear(true, false);
        loginList_.Clear();
    }
    // Or if we were running a server, stop it
    else if (network->IsServerRunning())
    {
        network->StopServer();
        scene_->Clear(true, false);
    }
}

void Server::SubscribeToEvents()
{
    // Subscribe to network events
    SubscribeToEvent(E_SERVERCONNECTED, URHO3D_HANDLER(Server, HandleConnectionStatus));
    SubscribeToEvent(E_SERVERDISCONNECTED, URHO3D_HANDLER(Server, HandleConnectionStatus));
    SubscribeToEvent(E_CONNECTFAILED, URHO3D_HANDLER(Server, HandleConnectionStatus));
    SubscribeToEvent(E_CLIENTCONNECTED, URHO3D_HANDLER(Server, HandleClientConnected));
    SubscribeToEvent(E_CLIENTDISCONNECTED, URHO3D_HANDLER(Server, HandleClientDisconnected));
    // This is a custom event, sent from the server to the client. It tells the node ID of the object the client should control
    SubscribeToEvent(E_CLIENTOBJECTID, URHO3D_HANDLER(Server, HandleClientObjectID));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_CLIENTOBJECTID);

    // Additional events that we might be interested in
    SubscribeToEvent(E_CLIENTIDENTITY, URHO3D_HANDLER(Server, HandleClientIdentity));
    SubscribeToEvent(E_CLIENTSCENELOADED, URHO3D_HANDLER(Server, HandleClientSceneLoaded));
//    SubscribeToEvent(E_NETWORKUPDATESENT, URHO3D_HANDLER(Server, HandleNetworkUpdateSent));
}

void Server::SendStatusMsg(StringHash msg)
{
    using namespace ServerStatus;

    // Send the event forward
    VariantMap& newEventData = GetEventDataMap();
    newEventData[P_STATUS] = msg;
    SendEvent(E_SERVERSTATUS, newEventData);
}

// SERVER: Send clients refreshed login list
void Server::SendLoginListRefreshMsg(Connection* connection)
{
    using namespace LoginListRefresh;

    // Send the event forward
    VariantMap &newEventData = GetEventDataMap();
    String packedStr = "";

    // Added players
    for (String login : loginList_.Keys()) {
        packedStr += login;
        packedStr += ";"; // delimiter

    }

    // Added ai bots
    for (String login : aiBotLoginList_) {
        packedStr += "[BOT] ";
        packedStr += login;
        packedStr += ";"; // delimiter
    }

    newEventData[P_LOGINLIST] = packedStr;
    connection->SendRemoteEvent(E_LOGINLISTREFRESH, true, newEventData);
    String clientId = connection->ToString().CString();

    URHO3D_LOGINFOF("E_LOGINLISTREFRESH -> %s to -> %s", packedStr.CString(), clientId.CString());
}


void Server::SendLoginListRefreshToClients() {
    // Send refreshed login list to clients
    Vector<Connection*> connList = loginList_.Values();
    for (Connection *c : connList) {
        if (c) {
            SendLoginListRefreshMsg(c);
        }
    }
}


// SERVER: Send clients chat message
void Server::SendChatMsg(Connection* connection, String user, String message)
{
    using namespace ChatMessage;

    // Send the event forward
    VariantMap &newEventData = GetEventDataMap();
    newEventData[P_USER] = user;
    newEventData[P_MESSAGE] = message;
    connection->SendRemoteEvent(E_CHATMESSAGE, true, newEventData);
}

// SERVER: Send clients next radio track
void Server::SendRadioTrackNextMsg(Connection* connection, String trackArtist, String trackArtistLink, String trackName)
{
    using namespace RadioTrackNext;

    // Send the event forward
    VariantMap &newEventData = GetEventDataMap();
    newEventData[P_TRACKARTIST] = trackArtist;
    newEventData[P_LINKARTIST] = trackArtistLink;
    newEventData[P_TRACKNAME] = trackName;
    connection->SendRemoteEvent(E_RADIOTRACKNEXT, true, newEventData);
}

// SERVER: Manage incoming clients
void Server::HandleClientIdentity(StringHash eventType, VariantMap& eventData)
{
    // SERVER CODE

	using namespace ClientIdentity;
    //URHO3D_LOGINFO("HandleClientIdentity");
    // When a client connects, assign to scene to begin scene replication
    Connection* newConnection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());


    String username = String(newConnection->GetIdentity()["UserName"]);
    String name = newConnection->identity_["UserName"].GetString();
    int colorIdx = newConnection->identity_["ColorIdx"].GetInt();
    Vector3 clientReqPos = newConnection->identity_["Position"].GetVector3();

    // Store login list on server
    loginList_[username] = newConnection;

    // Pack data of the object's nodes using a remote event
    VariantMap remoteEventData;
    remoteEventData[ClientObjectID::P_NETWORKACTOR] = 0; //networkActorNodeId_;
    remoteEventData[ClientObjectID::P_VEHICLE] = 0;//vehicleNodeId_;
    newConnection->SendRemoteEvent(E_CLIENTOBJECTID, true, remoteEventData);
    //URHO3D_LOGINFOF("SERVER -> HandleClientIdentity: sent to client -> actor [%l] , vehicle [%l]", networkActorNodeId_, vehicleNodeId_);

    // Send refreshed login list to clients
    Vector<Connection*> serverClientConnectionList = loginList_.Values();
    for (Connection *c : serverClientConnectionList) {
        if (c) {
            SendLoginListRefreshMsg(c);
        }
    }

    // Output the updated login list
    OutputLoginListToConsole();
}

void Server::HandleClientSceneLoaded(StringHash eventType, VariantMap& eventData)
{
//	using namespace ClientSceneLoaded;
 //   URHO3D_LOGINFO("HandleClientSceneLoaded");
 //   URHO3D_LOGINFOF("Server: Scene checksum -> %s", ToStringHex(scene_->GetChecksum()).CString());
}

void Server::HandleConnectionStatus(StringHash eventType, VariantMap& eventData)
{
    SendStatusMsg(eventType);
}

void Server::HandleClientConnected(StringHash eventType, VariantMap& eventData)
{
    using namespace ClientConnected;
}


void Server::HandleClientDisconnected(StringHash eventType, VariantMap& eventData)
{
    using namespace ClientConnected;
    URHO3D_LOGINFO("HandleClientDisconnected");

    // (On server)

    // When a client disconnects, remove the controlled object
    Connection* connection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());

    Vector<Connection*> connectList = loginList_.Values();

    URHO3D_LOGINFO("**** FINDING CONNECTION TO REMOVE FROM LOGIN LIST");

    for (int i = 0; i < connectList.Size(); i++) {
        if (connectList[i] == connection) {
            // Match
            URHO3D_LOGINFOF("**** MATCH: %s", loginList_.Keys()[i].CString());

            // Remove login from list
            loginList_.Erase(loginList_.Keys()[i]);
            break;
        }
    }

    OutputLoginListToConsole();

    URHO3D_LOGINFO("**** HandleClientDisconnected COMPLETED");

}

void Server::HandleClientObjectID(StringHash eventType, VariantMap& eventData)
{
    // CLIENT CODE (Called before the algebrakart HandleClientObjectID)

    if (eventType == E_CLIENTOBJECTID) {
        // CLIENT: Read data and store client object id information
        //networkActorNodeId_ = eventData[ClientObjectID::P_NETWORKACTOR].GetUInt();
        //vehicleNodeId_ = eventData[ClientObjectID::P_VEHICLE].GetUInt();
/*
        URHO3D_LOGINFOF(
                "CLIENT -> Server::HandleClientObjectID: networkActorNodeId_[%l], vehicleNodeId_[%l]",
                networkActorNodeId_, vehicleNodeId_);*/
    }

}

void Server::OutputLoginListToConsole() {

    URHO3D_LOGINFO("**** SERVER: CLIENT LIST *****************************************************");

    Vector<Connection*> connectList = loginList_.Values();

    for (int i = 0; i < connectList.Size(); i++) {
        URHO3D_LOGINFOF("**** LOGIN: %s", loginList_.Keys()[i].CString());
    }

    URHO3D_LOGINFO("******************************************************************************");

}

const SharedPtr<Recorder> &Server::GetRecorder() const {
    return recorder_;
}

void Server::SetRecorder(const SharedPtr<Recorder> &recorder) {
    recorder_ = recorder;
}
