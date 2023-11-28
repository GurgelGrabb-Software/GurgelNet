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

#include <cstdint>

class INetMessageProcessor;

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
	virtual uint8_t GetNetID() const = 0;

	/// <summary>
	/// Register a processor for handling custom message categories
	/// </summary>
	/// <param name="processor">Pointer to the processor you wish to register. The NetLayer assumes ownership of this pointer.</param>
	virtual void RegisterProcessor(INetMessageProcessor* processor) = 0;
};