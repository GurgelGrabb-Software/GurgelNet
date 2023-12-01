// GurgelNet - 2023
// ------------------------------------------------------------
// ConnectState.h
// ------------------------------------------------------------
// Enum for the different states a net layer may be in
// ------------------------------------------------------------

#pragma once

enum EConnectState
{
	EConnectState_Inactive,				// Not yet started
	EConnectState_PendingConnect,		// Awaiting confirmation of connection
	EConnectState_FinalizingConnection,	// Connection established but still in handshake phase
	EConnectState_Connected,			// Connected to the networking
};