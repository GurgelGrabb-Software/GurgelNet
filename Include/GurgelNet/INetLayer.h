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

#include <cstdint>

class INetMessageProcessor;
class INetMessageQueue;

class NETWORK_API INetLayer
{
public:
	virtual ~INetLayer() = default;

	/// <summary>
	/// Register a callback with this layer
	/// </summary>
	/// <param name="t">The ENetLayerCallback type</param>
	/// <param name="fPtr">Function pointer to invoke on this event</param>
	virtual void SetLayerCallback(ENetLayerCallback t, void* fPtr) = 0;

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
	/// Get the message queue belonging to this layer
	/// </summary>
	/// <returns>Reference to the INetMessageQueue</returns>
	virtual INetMessageQueue& MessageQueue() = 0;
};