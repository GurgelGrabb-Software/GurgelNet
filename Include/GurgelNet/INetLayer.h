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
#include "GurgelNet/Objects/INetObjectFactory.h"

#include "GurgelNet/Messages/INetMessage.h"

#include <cstdint>

class INetMessageProcessor;
class INetMessageQueue;

class NETWORK_API INetLayer
{
public:
	virtual ~INetLayer() = default;

	// ----------------------------------------------------------------------
	// Information Getters
	// ----------------------------------------------------------------------

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

	// ----------------------------------------------------------------------
	// Networking
	// ----------------------------------------------------------------------

	/// <summary>
	/// Send a network message through this Network Layer to the default target
	/// On server: ClientID_AllClients
	/// On cliend: ClientID_Server
	/// </summary>
	/// <param name="message">The message to send</param>
	/// <param name="reliable">True if this message should be reliable</param>
	virtual void Send(const INetMessage& message, bool reliable = false) = 0;

	/// <summary>
	/// Send a network message through this Network Layer to the specified targets
	/// </summary>
	/// <param name="message">The message to send</param>
	/// <param name="targetMask">ClientID mask to send the message to</param>
	/// <param name="reliable">True if this message should be reliable</param>
	virtual void Send(const INetMessage& message, ClientID targetMask, bool reliable = false) = 0;

	/// <summary>
	/// Spawn an object on the network.
	/// </summary>
	/// <param name="spawn">The object to spawn</param>
	virtual void SpawnNetworkObject(CNetObject& spawn) = 0;
};