#pragma once

#include "CSP_Server.h"
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
Client side prediction server.

- receive inputs from clients
- keep track of each client's last input ID
- sends last used input ID
- sends state snapshot
*/
struct CSP_Server : Component
{
	URHO3D_OBJECT(CSP_Server, Component);

	CSP_Server(Context* context);

	using ID = unsigned;

	// Register object factory and attributes.
	static void RegisterObject(Context* context);

	// Clients' inputs
	HashMap<Connection*, Controls> clientInputs;
	HashMap<Connection*, ID> clientLastIDs; // updated when the input is actually applied
	
	StateSnapshot snapshot;
    Vector<StateSnapshot> stateSnapshots_;

	// for debugging
	unsigned usedInputs = 0; // number of used inputs

	// Update time interval
	//TODO same as timestep?
	float updateInterval_ = 1.f / 30.f;	// default to 30 FPS
	// Update time accumulator
	float updateAcc_ = 0;


	// Read input sent from the client and apply it
	void readInput(Connection* connection, MemoryBuffer& message);


protected:
	VectorBuffer state;

	// send snapshot at network update FPS
	void HandleRenderUpdate(StringHash eventType, VariantMap& eventData);
	// Handle custom network messages
	void HandleNetworkMessage(StringHash eventType, VariantMap& eventData);


	/*
	serialization structure:
	- Last input ID
	- state snapshot
	*/
	// Prepare state snapshot for each networked scene
	void prepareStateSnapshot();
	// For each connection send the last received input ID and scene state snapshot
	void sendStateUpdates();
	// Send a state update to a given connection
	void sendStateUpdate(Connection* connection);
};
