#include "Src/Include/Objects/ServerObjectHandler.h"

#include "GurgelNet/Objects/INetObjectFactory.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/NetLayerMessages/LateJoinPayload.h"

#include "Src/Core/Logging.h"

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

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_objects.NumObjects());
	NETLOG_SERVER(ENetLogLevel_Message, "Spawned object. Assigned net ID {}", object.GetNetObjectID());
}

// ------------------------------------------------------------

void CServerObjectHandler::DespawnObject(CNetObject& object)
{
	const NetObjectID id = object.GetNetObjectID();

	if (object.IsOwner(ClientID_Server))
	{
		NETLOG_SERVER(ENetLogLevel_Message, "Despawning object with net ID {}", object.GetNetObjectID());

		CObjectMsg_Despawn despawnMsg;
		despawnMsg.id = id;
		_netContext.layer.msgQueuePtr->Send(despawnMsg, ClientID_AllClients, true);

		_objects.Remove(id);
		object.OnNetworkDespawn();
	}

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_objects.NumObjects());
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

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_objects.NumObjects());
	NETLOG_SERVER(ENetLogLevel_Message, "Spawned object requested by client. Assigned net ID {}", objectPtr->GetNetObjectID());
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessObjectDespawn(ClientID requestingClient, CObjectMsg_Despawn& despawnMsg)
{
	if (!_objects.HasObject(despawnMsg.id)) return; 
	CNetObject* object = _objects.GetObject(despawnMsg.id).objectPtr;
	if (!object->IsOwner(requestingClient)) return; // If this client is not the owner, no need to do anything

	// Send out the despawn notification to all clients except the one making the despawn request
	_netContext.layer.msgQueuePtr->Send(despawnMsg, ClientMask_Remove(ClientID_AllClients, requestingClient), true); 
	
	_objects.Remove(object->GetNetTypeID());
	object->OnNetworkDespawn();
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessNetVarSync(CObjectMsg_NetVarSync& syncMsg)
{
	const NetObjectID objID = syncMsg.objectID;
	const NetVarID varID = syncMsg.varID;

	if (!_objects.HasObject(objID)) return;
	CNetworkVariable* varPtr = _objects.GetObject(objID).netVariables.GetVariable(varID);
	syncMsg.DeserializeNetVarData(*varPtr);
}

// ------------------------------------------------------------

void CServerObjectHandler::ProcessNetFuncCall(CObjectMsg_NetFuncCall& callMsg)
{
	const NetObjectID objID = callMsg.objectID;
	const NetFuncID funcID = callMsg.functionID;
	if (!_objects.HasObject(objID)) return;
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