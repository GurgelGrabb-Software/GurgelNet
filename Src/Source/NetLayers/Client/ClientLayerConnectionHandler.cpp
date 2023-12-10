#include "Src/Include/NetLayers/Client/ClientLayerConnectionHandler.h"

#include "Src/Include/NetLayers/NetLayerCallbackStore.h"
#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/NetLayerMessages/ConnectionMessages.h"

#include "Src/Include/NetLayers/Client/NetLayerClient.h"
#include "Src/Include/NetLayers/NetLayerContext.h"

#include "Src/Core/Logging.h"

CClientLayerConnectionHandler::CClientLayerConnectionHandler(CNetLayerClient& clientLayer, SNetLayerContext& context)
	: _clientLayer(clientLayer)
	, _netContext(context)
{
	NETLOG_CLIENT(ENetLogLevel_Confirm, "Client layer created");
}

void CClientLayerConnectionHandler::RecieveAssignedID(ClientID assignedID)
{
	_netContext.layer.layerNetworkID = assignedID;
	_clientLayer.ChangeClientState(EConnectState_FinalizingConnection);

	CConnectMsg_ConfirmID confirmMsg;

	// If there is a callback to write any approval data, that should go with our confirmation here
	if (FNetWrite approvalWriteF = _netContext.layer.callbackStorePtr->GetCallbackAs<FNetWrite>(ENetLayerCallback_ClientApproveRequestWrite))
	{
		confirmMsg.approvalRequestWrite = approvalWriteF;
	}

	// Send off the confirmation message
	_netContext.layer.msgQueuePtr->Send(confirmMsg, ClientID_Server, true);
	NETLOG_CLIENT(ENetLogLevel_Message, "Recieved assigned client ID {}", assignedID);
}

void CClientLayerConnectionHandler::RecieveLateJoinData(INetMessageReader& lateJoinReader)
{
	FNetRead lateJoinReadF = _netContext.layer.callbackStorePtr->GetCallbackAs<FNetRead>(ENetLayerCallback_ClientLateJoinSync);

	CConnectMsg_LateJoin lateJoinMsg;
	lateJoinMsg.lateJoinRead = lateJoinReadF;
	lateJoinReader.Read(lateJoinMsg);

	NETLOG_CLIENT(ENetLogLevel_Message, "Recieved late join message containing {} objects", lateJoinMsg.lateJoinPayload.numObjects);

	_clientLayer.ObjectHandler().ProcessLateJoinPayload(lateJoinMsg.lateJoinPayload);

	// Send the confirmation of having joined
	_netContext.layer.msgQueuePtr->Send(CConnectMsg_LateJoinConfirm(), ClientID_Server, true);
}

void CClientLayerConnectionHandler::RecieveConnectionConfirmation()
{
	NETLOG_CLIENT(ENetLogLevel_Confirm, "Connection active!");

	_clientLayer.ChangeClientState(EConnectState_Connected);
}
