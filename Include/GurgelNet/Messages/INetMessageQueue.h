// GurgelNet - 2023
// ------------------------------------------------------------
// INetMessageQueue.h 
// ------------------------------------------------------------
// Interface for the net message queue type
// 
// This is the core type to send messages via a network layer
// 
// To recieve messages, see INetMessageProcessor
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/Core/NetTypes.h"

class INetMessage;

class INetMessageQueue
{
public:

	/// <summary>
	/// Send the given message to the default target (If client, this is the server. If server, this is all clients)
	/// </summary>
	/// <param name="customMessage">The message to send</param>
	/// <param name="reliable">Should this message be reliable?</param>
	virtual void Send(const INetMessage& customMessage, bool reliable) = 0;

	/// <summary>
	/// Send the given message to custom target(s)
	/// </summary>
	/// <param name="customMessage">The message to send</param>
	/// <param name="targetMast">Bitmask of targets to send to</param>
	/// <param name="reliable">Should this message be reliable?</param>
	virtual void Send(const INetMessage& customMessage, ClientID targetMask, bool reliable) = 0;
};