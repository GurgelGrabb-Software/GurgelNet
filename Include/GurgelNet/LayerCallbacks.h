// GurgelNet - 2023
// ------------------------------------------------------------
// LayerCallbacks.h 
// ------------------------------------------------------------
// Enum and typedefs for layer callbacks
// 
// These are bound to a specific INetLayer unlike NetSettings 
// which are bound to the full GurgelNet library
// ------------------------------------------------------------

#pragma once

#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Serialization/INetSerializer.h"

// ------------------------------------------------------------

typedef void(*FBasicCallback)();
typedef void(*FClientConnection)(ClientID);
typedef void(*FNetWrite)(INetMessageWriter&);
typedef void(*FNetRead)(INetMessageReader&);
typedef bool(*FApproveClient)(INetMessageReader&);

// ------------------------------------------------------------

enum ENetLayerCallback
{
	ENetLayerCallback_LocalConnected,				// The layer is connected and ready to use. The layer may be unsafe to use before this has been called.
	ENetLayerCallback_LocalDisconnected,			// The layer has disconnected and is now inactive.

	ENetLayerCallback_ClientConnect,				// A new client connected [void(ClientID)] 
	ENetLayerCallback_ClientDisconnect,				// A client disconnected [void(ClientID)] 

	ENetLayerCallback_ServerApproveClient,			// Approve or deny a client based on optional data (if unset, accepts all until at capacity) [bool(ClientID, INetMessageReader&)] 
	ENetLayerCallback_ServerSyncLateClient,			// Callback to write data on server that will be sent to a connected client to do any syncing of prior state [void(INetMessageWriter&)]

	ENetLayerCallback_ClientApproveRequestWrite,	// Write any additional data that will be used by the server to determine if to approve this client [void(INetMessageWriter&)] 
	ENetLayerCallback_ClientLateJoinSync,			// Callback to read any late join sync data

	ENetLayerCallback_Count,
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
