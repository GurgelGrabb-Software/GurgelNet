#include "Src/Include/NetLayers/Client/ClientLayerConnectionHandler.h"

#include "Src/Include/NetLayers/NetLayerCallbackStore.h"
#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/NetLayerMessages/ConnectionMessages.h"

#include "Src/Include/NetLayers/Client/NetLayerClient.h"
#include "Src/Include/NetLayers/NetLayerContext.h"

CClientLayerConnectionHandler::CClientLayerConnectionHandler(CNetLayerClient& clientLayer, SNetLayerContext& context)
	: _clientLayer(clientLayer)
	, _netContext(context)
{
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
}

void CClientLayerConnectionHandler::RecieveLateJoinData(INetMessageReader& lateJoinReader)
{
	if (FNetRead lateJoinReadF = _netContext.layer.callbackStorePtr->GetCallbackAs<FNetRead>(ENetLayerCallback_ClientLateJoinSync))
	{
		lateJoinReadF(lateJoinReader);
	}

	// Send the confirmation of having joined
	_netContext.layer.msgQueuePtr->Send(CConnectMsg_LatJoinConfirm(), ClientID_Server, true);
}

void CClientLayerConnectionHandler::RecieveConnectionConfirmation()
{
	_clientLayer.ChangeClientState(EConnectState_Connected);
}
