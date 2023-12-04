#include "Src/Include/Objects/ServerObjectHandler.h"

#include "GurgelNet/Objects/INetObjectFactory.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/NetLayerMessages/LateJoinPayload.h"

// ------------------------------------------------------------

CServerObjectHandler::CServerObjectHandler(SNetLayerContext& netContext)
	: CSharedObjectHandler(netContext)
	, _objects()
{
}

// ------------------------------------------------------------

void CServerObjectHandler::SpawnObject(CNetObject& object)
{
	NetObjectID assignedID = _objects.Insert(&object);
	SNetObjectHandle& objHandle = _objects.GetObject(assignedID);

	CNetObjectInitializer initializer(objHandle);
	object.OnNetworkSpawn(initializer);

	CObjectMsg_Spawn spawnMsg;
	spawnMsg.objectHandle = &objHandle;
	_netContext.layer.msgQueuePtr->Send(spawnMsg, ClientID_AllClients, true);
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessObjectSpawnRequest(ClientID requestingClient, CObjectMsg_SpawnRequest& requestMsg)
{
 	CNetObject* objectPtr = ObjectFactory()->MakeObject(requestMsg.spawnedTypeID);
	NetObjectID assignedID = _objects.Insert(objectPtr);
	SNetObjectHandle& objHandle = _objects.GetObject(assignedID);

	requestMsg.ReadPreSpawnData(objHandle);

	CNetObjectInitializer initializer(objHandle);
	objectPtr->OnNetworkSpawn(initializer);

	CObjectMsg_SpawnConfirm confirmMsg;
	confirmMsg.pendingID = requestMsg.pendingSpawnID;
	confirmMsg.confirmedID = assignedID;
	
	CObjectMsg_Spawn spawnMsg;
	spawnMsg.objectHandle = &objHandle;

	_netContext.layer.msgQueuePtr->Send(confirmMsg, requestingClient, true);
	_netContext.layer.msgQueuePtr->Send(spawnMsg, ClientID_AllExcept(ClientID_Server | requestingClient), true);
}

void CServerObjectHandler::ProcessNetVarSync(CObjectMsg_NetVarSync& syncMsg)
{
	const NetObjectID objID = syncMsg.objectID;
	const NetVarID varID = syncMsg.varID;

	CNetworkVariable* varPtr = _objects.GetObject(objID).netVariables.GetVariable(varID);
	syncMsg.DeserializeNetVarData(*varPtr);
}

// ------------------------------------------------------------

void CServerObjectHandler::WriteLateJoinPayload(CLateJoinPayload& payload)
{
	_objects.ForEach([&payload](auto& h) { payload.AddObject(h); });
}

// ------------------------------------------------------------

void CServerObjectHandler::RunNetObjectUpdate()
{
	_objects.ForEach([&](auto& h) {UpdateNetObject(h); });
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------