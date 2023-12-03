#pragma once

enum EConnectMsg
{
	EConnectMsg_AssignClientID,		// Server telling client its assigned client ID
	EConnectMsg_ConfirmClientID,	// Client acknowledging to server it has received its client ID (also may contain additional data to be used for client approval)

	EConnectMsg_LateJoinSync,		// Server sending optional late join data to client
	EConnectMsg_LateJoinComplete,	// Client notifying server it has finished processing late join data

	EConnectMsg_ConnectionActive,	// Server notifying client it is now active
};

enum EObjectMsg
{

};