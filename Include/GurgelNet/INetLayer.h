// GurgelNet - 2023
// ------------------------------------------------------------
// INetLayer.h 
// ------------------------------------------------------------
// NetLayer is the core type representing a sender/listener
// on the network.
// 
// Create these through the GurgelNet_Create- functions
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/ConnectState.h"
#include "GurgelNet/LayerCallbacks.h"

#include "GurgelNet/Core/NetTypes.h"

#include "GurgelNet/Objects/NetObject.h"
#include "GurgelNet/Objects/NetObjectOwnership.h"

#include "GurgelNet/Messages/INetMessage.h"

#include <cstdint>

// ------------------------------------------------------------

class INetMessageProcessor;
class INetMessageQueue;

// ------------------------------------------------------------

class NETWORK_API INetLayer
{
public:
	virtual ~INetLayer() = default;

	/// <summary>
	/// Get the current state of this Layer
	/// </summary>
	/// <returns>Inactive, PendingConnect or Connected</returns>
	virtual EConnectState CurrentState() const = 0;

	/// <summary>
	/// Get the ID of this Layer
	/// </summary>
	/// <returns>1 if server, 0 if inactive, > 1 if connected as client (id assigned by server)</returns>
	virtual ClientID GetNetID() const = 0;

	/// <summary>
	/// Register a processor for handling custom message categories
	/// </summary>
	/// <param name="processor">Pointer to the processor you wish to register. The NetLayer assumes ownership of this pointer.</param>
	virtual void RegisterProcessor(INetMessageProcessor* processor) = 0;

	/// <summary>
	/// Get the message queue used to send messages from this layer
	/// </summary>
	/// <returns>Reference to the message queue</returns>
	virtual INetMessageQueue& MessageQueue() = 0;

	/// <summary>
	/// Spawn the provided object on the network
	/// </summary>
	/// <param name="spawn">The object to spawn</param>
	/// <param name="owner">The owner of this object (defaults to server)</param>
	virtual void SpawnNetworkObject(CNetObject& spawn, ENetObjectOwner owner = ENetObjectOwner::Server) = 0;

	/// <summary>
	/// Despawn the provided object if it is currently active on network
	/// </summary>
	/// <param name="despawn">The object to despawn</param>
	virtual void DespawnNetworkObject(CNetObject& despawn) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------