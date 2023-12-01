#include "ServerInternalMessageProcessor.h"
#include "Src/ServerLayer.h"
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Serialization/INetSerializer.h"

void ProcessIDConfirmation(uint8_t clientID, CServerLayer& layer, INetMessageReader& reader)
{
	// This message also may contain any additional information used to approve this client
	CInternalMsg_Client_ConfirmIDAndRequestApproval msg;

	void* approveClientFPtr = nullptr;
	layer.TryGetCallbackPtr(ENetLayerCallback_ServerApproveClient, &approveClientFPtr);
	msg.F_serverApproveClient = ((FApproveClient)approveClientFPtr);

	reader.Read(msg);

	layer.ApproveClientConnection(clientID, msg.clientApprovalState);
}

void ProcessLateJoinConfirmation(uint8_t clientID, CServerLayer& layer)
{
	layer.FinalizeApprovedClientConnection(clientID);
}

CServerInternalMessageProcessor::CServerInternalMessageProcessor(CServerLayer& layer)
	: _layer(layer)
{
}

uint8_t CServerInternalMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Internal;
}

void CServerInternalMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EInternalMsg_ClientToServer_ID: ProcessIDConfirmation(header.senderID, _layer, reader); break;
	case EInternalMsg_ClientToServer_LateJoinConfirm: ProcessLateJoinConfirmation(header.senderID, _layer); break;
	default:
		break;
	}
}
