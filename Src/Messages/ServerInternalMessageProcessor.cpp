#include "ServerInternalMessageProcessor.h"
#include "GurgelNet/Serialization/INetSerializer.h"

#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/NetMessageHeader.h"

#include "Src/ServerLayer.h"
#include "Src/Messages/ObjectMessageTypes.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "Src/Messages/NetVarMessages.h"

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

void ProcessObjectSpawnRequest(const SNetMessageHeader& header, INetMessageReader& reader, CServerLayer& layer)
{
	CInternalMsg_Object_ClientRequestSpawn spawnRequestMsg;
	reader.Read(spawnRequestMsg);

	// Spawn the object using a registered factory method
	INetObjectFactory& objectFactory = layer.GetObjectFactory();
	CNetObject* madeObject = objectFactory.MakeObject(spawnRequestMsg.ObjectTypeID);
	madeObject->ReadSpawnData(reader);

	// With this object created, we can now spawn it on the server
	layer.ProcessSpawnRequest(*madeObject, header.senderID, spawnRequestMsg.PendingID);
}

void RunNetVarSync(const SNetMessageHeader& header, INetMessageReader& reader, CServerLayer& layer)
{
	CInternalMsg_NetVarSync netVarMsg;
	reader.Read(netVarMsg);

	CNetworkVariable* variable = layer.GetNetVar(netVarMsg.ObjectID, netVarMsg.VarID);
	variable->Deserialize(reader);

	// Now sync change to all other clients
	layer.Send(netVarMsg, ClientMask_Remove(ClientID_All, header.senderID), variable->SyncReliable());
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

	case EInternalMsg_Object_ClientRequestSpawn: ProcessObjectSpawnRequest(header, reader, _layer); break;
	case EInternalMsg_NetVarSync: RunNetVarSync(header, reader, _layer); break;
	default:
		break;
	}
}
