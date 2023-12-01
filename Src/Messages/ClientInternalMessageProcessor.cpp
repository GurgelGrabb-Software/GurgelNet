#include "ClientInternalMessageProcessor.h"
#include "Src/ClientLayer.h"
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Serialization/INetSerializer.h"

void ProcessIDRecieve(CClientLayer& layer, INetMessageReader& reader, INetMessageQueue& msgQueue)
{
	uint8_t assignedID;
	reader.Read(assignedID);
	layer.AssignNetID(assignedID);
	
	CInternalMsg_Client_ConfirmIDAndRequestApproval confirmAndRequestApproval;
	
	void* clientApprovalWriteFPtr = nullptr;
	layer.TryGetCallbackPtr(ENetLayerCallback_ClientApproveRequestWrite, &clientApprovalWriteFPtr);
	confirmAndRequestApproval.F_clientWriteApproveRequest = ((FNetWrite)clientApprovalWriteFPtr);

	msgQueue.Send(confirmAndRequestApproval, ClientID_Server, true); // Send confirmation
}

void ProcessLateJoinSync(INetMessageReader& reader, CClientLayer& layer)
{
	void* lateJoinReadFuncPtr;
	if (layer.TryGetCallbackPtr(ENetLayerCallback_ClientLateJoinSync, &lateJoinReadFuncPtr))
	{
		CInternalMsg_Server_LateJoinSync syncMsg;
		syncMsg.clientRead = (FNetRead)lateJoinReadFuncPtr;
		reader.Read(syncMsg);
	}
	
	// Confirm to server that we are synced
	layer.MessageQueue().Send(SHeaderOnlyMsg(EInternalMsg_ClientToServer_LateJoinConfirm), true);
}


CClientInternalMessageProcessor::CClientInternalMessageProcessor(CClientLayer& layer)
	: _layer(layer)
{
}

uint8_t CClientInternalMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Internal;
}

void CClientInternalMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EInternalMsg_ServerToClient_ID: ProcessIDRecieve(_layer, reader, netLayer.MessageQueue()); break;
	case EInternalMsg_ServerToClient_LateJoinSync: ProcessLateJoinSync(reader, _layer); break;
	case EInternalMsg_ServerToClient_NotifyFinalized: _layer.ConnectionFinalized(); break;
		break;
	}
}
