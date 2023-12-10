#include "Src/Include/NetLayers/Server/ServerLayerConnectionsHandler.h"

#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/NetLayers/NetLayerCallbackStore.h"

#include "Src/Include/NetLayerMessages/ConnectionMessages.h"

#include "Src/Include/NetLayers/Server/NetLayerServer.h"
#include "Src/Include/Objects/ServerObjectHandler.h"

#include "GurgelNet/Messages/INetMessageQueue.h"
#include "GurgelNet/LayerCallbacks.h"

#include "Src/Core/Logging.h"

// ------------------------------------------------------------

CServerLayerConnectionsHandler::CServerLayerConnectionsHandler(CNetLayerServer& serverLayer, SNetLayerContext& netContext)
	: _serverLayer(serverLayer)
	, _netContext(netContext)
	, _connections(netContext)
{
}

// ------------------------------------------------------------

void CServerLayerConnectionsHandler::IncomingConnection(unsigned int hConnection)
{
	ClientID foundID;
	if (_connections.TryFindFreeID(foundID))
	{
		_connections.OpenConnection(hConnection, foundID);
	}
}

// ------------------------------------------------------------

void CServerLayerConnectionsHandler::EstablishedConnection(unsigned int hConnection)
{
	auto connectionPtr = _connections.GetConnectionByHandle(hConnection);
	
	NETLOG_SERVER(ENetLogLevel_Confirm, "Connection for client with ID {} established", connectionPtr->clientID);

	CConnectMsg_AssignID assignIDMsg;
	assignIDMsg.id = connectionPtr->clientID;

	_netContext.layer.msgQueuePtr->Send(assignIDMsg, connectionPtr->clientID, true);
}

// ------------------------------------------------------------

void CServerLayerConnectionsHandler::ClosedConnection(unsigned int hConnection)
{
	_connections.CloseConnectionByHandle(hConnection);
}

// ------------------------------------------------------------

void CServerLayerConnectionsHandler::RunClientApproval(ClientID clientID, INetMessageReader& approvalDataReader)
{
	auto callbacks = _netContext.layer.callbackStorePtr;
	FApproveClient clientApprovalFunction = callbacks->GetCallbackAs<FApproveClient>(ENetLayerCallback_ServerApproveClient);
	FNetWrite lateJoinDataWriteFunction = callbacks->GetCallbackAs<FNetWrite>(ENetLayerCallback_ClientLateJoinSync);

	// If there is an approval function, run it now and close the connection on failure
	if (clientApprovalFunction && !clientApprovalFunction(approvalDataReader))
	{
		// This means the client was not approved so we close the connection and early out
		_connections.CloseConnectionByID(clientID);
		return;
	}

	CConnectMsg_LateJoin msg;
	msg.lateJoinWrite = lateJoinDataWriteFunction;
	_serverLayer.ObjectHandler().WriteLateJoinPayload(msg.lateJoinPayload);
	_netContext.layer.msgQueuePtr->Send(msg, clientID, true);
}

// ------------------------------------------------------------

void CServerLayerConnectionsHandler::LateJoinCompleted(ClientID id)
{
	// Mark the connection as active and notify the client that it should now expect full messaging
	_connections.SetConnectionActive(id);
	_netContext.layer.msgQueuePtr->Send(CConnectMsg_ConnectionActive(), id, true);
}

// ------------------------------------------------------------

const SServerLayerConnections& CServerLayerConnectionsHandler::GetConnections() const
{
	return _connections;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------