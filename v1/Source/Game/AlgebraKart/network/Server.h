#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Input/Controls.h>
#include "Vehicle.h"

namespace Urho3D
{
class Scene;
class Connection;
}

using namespace Urho3D;
//=============================================================================
//=============================================================================
class ClientObj;

//=============================================================================
//=============================================================================
// UDP port we will use
const unsigned short SERVER_PORT = 2345;

//=============================================================================
// SERVER EVENT MESSAGES
//=============================================================================
URHO3D_EVENT(E_SERVERSTATUS, ServerStatus)
{
	URHO3D_PARAM(P_STATUS, Status); // unsigned
}

URHO3D_EVENT(E_CLIENTOBJECTID, ClientObjectID)
{
    URHO3D_PARAM(P_NETWORKACTOR, NetworkActorID);         // unsigned
    URHO3D_PARAM(P_VEHICLE, VehicleID);         // unsigned
}

URHO3D_EVENT(E_PLAYERSPAWNED, PlayerSpawned)
{
    URHO3D_PARAM(P_PLAYERID, NetworkActorID);         // unsigned
}

URHO3D_EVENT(E_PLAYERLOADED, PlayerLoaded)
{
    URHO3D_PARAM(P_PLAYERID, NetworkActorID);         // unsigned
}


URHO3D_EVENT(E_LOGINLISTREFRESH, LoginListRefresh)
{
    URHO3D_PARAM(P_LOGINLIST, LoginList);         // String semi-colon delimited
}

URHO3D_EVENT(E_PLAYERSTATE, ClientPlayerState)
{
    URHO3D_PARAM(P_LIFE, Life);         // unsigned
    URHO3D_PARAM(P_RPM, RPM);         // float
    URHO3D_PARAM(P_VELOCITY, Velocity);     // float
    URHO3D_PARAM(P_PRANA, Prana);           // float
    URHO3D_PARAM(P_STEER, Steering);         // float
    URHO3D_PARAM(P_PICKUP_ITEM, PickUp);            // int
}

URHO3D_EVENT(E_PLAYERPICKUPSTATE, ClientPlayerPickUpState)
{
    URHO3D_PARAM(P_PICKUP_ITEM, PickUp);            // int
}

URHO3D_EVENT(E_BEATPULSE, ClientBeatState)
{
    URHO3D_PARAM(P_SEQ_CURR_TIME, CurrSeqTime);     // float
    URHO3D_PARAM(P_SEQ_BEAT_TIME, SeqBeatTime);     // float
    URHO3D_PARAM(P_SEQ_BEAT, SeqBeat);              // unsigned
}

URHO3D_EVENT(E_RACERANK, RaceRank)
{
    URHO3D_PARAM(P_PLAYERID_LIST, playerId); // // String semi-colon delimited of player id
    URHO3D_PARAM(P_RANK_LIST, rank); // String semi-colon delimited of rank (int)
    URHO3D_PARAM(P_TIME_LIST, time); // String semi-colon delimited of time (float)
}

URHO3D_EVENT(E_RACEVICTORY, RaceVictory)
{
    URHO3D_PARAM(P_PLAYERID_LIST, playerId); // // String semi-colon delimited of player id
    URHO3D_PARAM(P_RANK_LIST, rank); // String semi-colon delimited of rank (int)
    URHO3D_PARAM(P_TIME_LIST, time); // String semi-colon delimited of time (float)
}

URHO3D_EVENT(E_RADIOTRACKNEXT, RadioTrackNext)
{
    URHO3D_PARAM(P_TRACKARTIST, trackArtist); // String of artist name
    URHO3D_PARAM(P_LINKARTIST, trackArtistLink); // String of artist link
    URHO3D_PARAM(P_TRACKNAME, trackName); // String of track name
}

URHO3D_EVENT(E_CHATMESSAGE, ChatMessage)
{
    URHO3D_PARAM(P_USER, User); // String of user id
    URHO3D_PARAM(P_MESSAGE, Message); // String of chat message
}

//=============================================================================
// SERVER
//=============================================================================
class Server : public Object
{
    URHO3D_OBJECT(Server, Object);
public:

    Server(Context* context);
    virtual ~Server();
    
    void RegisterScene(Scene *scene);
    bool StartServer(unsigned short port);
   // bool Connect(const String &address, unsigned short port, const VariantMap& identity = Variant::emptyVariantMap);
    void Disconnect();

    void InitializeScene(File &file);
    void UpdateActors(float timeStep);

    HashMap<String, Connection*> GetLoginList() { return loginList_; }

    void SetAgents(Vector<String> list) {
        aiBotLoginList_ = list;
    }

    Vector<String> GetAgents() {
        return aiBotLoginList_;
    }

    void SendChatMsg(Connection* connection, String user, String message);
    void SendRadioTrackNextMsg(Connection* connection, String trackArtist, String trackArtistLink, String trackName);
    void SendLoginListRefreshMsg(Connection* connection);

    void SendLoginListRefreshToClients();

    const SharedPtr<Recorder> &GetRecorder() const;
    void SetRecorder(const SharedPtr<Recorder> &recorder);


protected:
    void SubscribeToEvents();
    void SendStatusMsg(StringHash msg);
    void SendPlayerStateMsg(Connection* connection);

    /// Handle connection status change (just update the buttons that should be shown.)
    void HandleConnectionStatus(StringHash eventType, VariantMap& eventData);
    /// Handle a client connecting to the server.
    void HandleClientConnected(StringHash eventType, VariantMap& eventData);
    /// Handle a client disconnecting from the server.
    void HandleClientDisconnected(StringHash eventType, VariantMap& eventData);
    /// Handle remote event from server which tells our controlled object node ID.
    void HandleNetworkUpdateSent(StringHash eventType, VariantMap& eventData);
    void HandleClientObjectID(StringHash eventType, VariantMap& eventData);
    void HandleClientIdentity(StringHash eventType, VariantMap& eventData);
    void HandleClientSceneLoaded(StringHash eventType, VariantMap& eventData);

    void OutputLoginListToConsole();

protected:
    SharedPtr<Scene> scene_;
    HashMap<String, Connection*> loginList_;
    Vector<String> aiBotLoginList_;

    SharedPtr<Recorder> recorder_;

};
