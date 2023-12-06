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

void CServerObjectHandler::SpawnObject(CNetObject& object, ENetObjectOwner owner)
{
	NetObjectID assignedID = _objects.Insert(&object);
	SNetObjectHandle& objHandle = _objects.GetObject(assignedID);

	ObjectFactory()->PreSpawn(object);

	CNetObjectInitializer initializer(_netContext, objHandle, NetObjectOwnerMask(owner, ClientID_Server));
	object.OnNetworkSpawn(initializer);

	ObjectFactory()->PostSpawn(object);

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

	ObjectFactory()->PreSpawn(*objectPtr);
	requestMsg.ReadPreSpawnData(objHandle);

	CNetObjectInitializer initializer(_netContext, objHandle, requestMsg.ownerID);
	objectPtr->OnNetworkSpawn(initializer);

	ObjectFactory()->PostSpawn(*objectPtr);

	// Add this as an object owned by this specific client in the case that its owned by the requesting client
	if (requestMsg.ownerID != ClientID_Server && requestMsg.ownerID != ClientID_AllClients && requestMsg.ownerID != ClientID_All)
	{
		_clientOwnedObjectIDs[requestingClient].push_back(assignedID);
	}

	CObjectMsg_SpawnConfirm confirmMsg;
	confirmMsg.pendingID = requestMsg.pendingSpawnID;
	confirmMsg.confirmedID = assignedID;
	
	CObjectMsg_Spawn spawnMsg;
	spawnMsg.objectHandle = &objHandle;

	_netContext.layer.msgQueuePtr->Send(confirmMsg, requestingClient, true);
	_netContext.layer.msgQueuePtr->Send(spawnMsg, ClientID_AllExcept(ClientID_Server | requestingClient), true);
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessNetVarSync(CObjectMsg_NetVarSync& syncMsg)
{
	const NetObjectID objID = syncMsg.objectID;
	const NetVarID varID = syncMsg.varID;

	CNetworkVariable* varPtr = _objects.GetObject(objID).netVariables.GetVariable(varID);
	syncMsg.DeserializeNetVarData(*varPtr);
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessNetFuncCall(CObjectMsg_NetFuncCall& callMsg)
{
	const NetObjectID objID = callMsg.objectID;
	const NetFuncID funcID = callMsg.functionID;

	_objects.GetObject(objID).netFuncList->InvokeFunction(funcID, callMsg.GetReader());
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