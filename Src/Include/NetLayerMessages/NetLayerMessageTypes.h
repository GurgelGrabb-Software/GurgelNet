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
	EObjectMsg_Spawn,				// Server notifying about the spawn of a network object
	EObjectMsg_SpawnRequest,		// Client requesting spawn of a network object
	EObjectMsg_SpawnConfirm,		// Server confirming spawn of a requested network object
	EObjectMsg_Despawn,				// An object has been despawned and should be removed

	EObjectMsg_NetVarSync,			// Sync of net variable state (message will come from an owner of this net var)

	EObjectMsg_NetFuncCall,			// Call via remote to a network function
};