#include "Src/Include/Objects/ClientObjectHandler.h"

#include "GurgelNet/Objects/INetObjectFactory.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Objects/NetObjectInitializer.h"

#include "Src/Include/NetLayerMessages/LateJoinPayload.h"

#include "Src/Core/Logging.h"

// ------------------------------------------------------------

CClientObjectHandler::CClientObjectHandler(SNetLayerContext& netContext)
	: CSharedObjectHandler(netContext)
	, _pendingObjects()
	, _activeObjects()
{
}

// ------------------------------------------------------------

void CClientObjectHandler::SpawnObject(CNetObject& object, ENetObjectOwner owner)
{
	NetObjectID pendingID = _pendingObjects.Insert(&object);

	NETLOG_CLIENT(ENetLogLevel_Message, "Sending spawn object request. Pending ID: {}", pendingID);

	CObjectMsg_SpawnRequest spawnRequestMsg;
	spawnRequestMsg.objectHandle = &_pendingObjects.GetObject(pendingID);
	spawnRequestMsg.ownerID = NetObjectOwnerMask(owner, _netContext.layer.layerNetworkID);

	_netContext.layer.msgQueuePtr->Send(spawnRequestMsg, ClientID_Server, true);

	ObjectFactory()->PreSpawn(object);
}

// ------------------------------------------------------------

void CClientObjectHandler::ObjectSpawnConfirmed(CObjectMsg_SpawnConfirm& confirmMsg)
{
	const NetObjectID pendingID = confirmMsg.pendingID;
	const NetObjectID confirmedID = confirmMsg.confirmedID;
	CNetObject* const objectPtr = _pendingObjects.GetObject(pendingID).objectPtr;

	// Remove from the pending objects
	_pendingObjects.Remove(pendingID);
	
	// Insert into active objects with the server-confirmed ID
	_activeObjects.InsertWithID(objectPtr, confirmedID);
	SNetObjectHandle& activeHandle = _activeObjects.GetObject(confirmMsg.confirmedID);

	// Run spawn
	CNetObjectInitializer initializer(_netContext, activeHandle, confirmMsg.ownerMask);
	activeHandle.objectPtr->OnNetworkSpawn(initializer);

	ObjectFactory()->PostSpawn(*objectPtr);

	NETLOG_CLIENT(ENetLogLevel_Message, "Requested spawn of pending ID {} was confirmed with ID {}", pendingID, confirmedID);

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_activeObjects.NumObjects());
}

// ------------------------------------------------------------

void CClientObjectHandler::ProcessObjectSpawn(CObjectMsg_Spawn& spawnMsg)
{
	const NetObjectID objectID = spawnMsg.spawnedObjectID;
	const NetTypeID objectTypeID = spawnMsg.spawnedTypeID;

	// Insert to object list
	CNetObject* const object = ObjectFactory()->MakeObject(objectTypeID);
	_activeObjects.InsertWithID(object, objectID);

	ObjectFactory()->PreSpawn(*object);

	// Read the object data (initialization data)
	SNetObjectHandle& activeHandle = _activeObjects.GetObject(objectID);
	spawnMsg.ReadPreSpawnData(activeHandle);

	// Run the spawn
	CNetObjectInitializer initializer(_netContext, activeHandle, spawnMsg.ownerID);
	object->OnNetworkSpawn(initializer);

	// Read post spawn data (initial net var values)
	spawnMsg.ReadPostSpawnData(activeHandle);

	ObjectFactory()->PostSpawn(*object);

	NETLOG_CLIENT(ENetLogLevel_Message, "Spawned network object of type ID {} with net object ID {}", objectTypeID, objectID);

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_activeObjects.NumObjects());
}

// ------------------------------------------------------------

void CClientObjectHandler::SyncNetVar(CObjectMsg_NetVarSync& syncMsg)
{
	const NetObjectID objID = syncMsg.objectID;
	const NetVarID varID = syncMsg.varID;

	CNetworkVariable* variable = _activeObjects.GetObject(objID).netVariables.GetVariable(varID);
	syncMsg.DeserializeNetVarData(*variable);
}

// ------------------------------------------------------------

void CClientObjectHandler::ProcessNetFuncCall(CObjectMsg_NetFuncCall& callMsg)
{
	const NetObjectID objID = callMsg.objectID;
	const NetFuncID funcID = callMsg.functionID;
	_activeObjects.GetObject(objID).netFuncList->InvokeFunction(funcID, callMsg.GetReader());
}

// ------------------------------------------------------------

void CClientObjectHandler::ProcessLateJoinPayload(CLateJoinPayload& payload)
{
	for (NetObjectID i = 0; i < payload.numObjects; ++i)
	{
		SLateJoinObjectPayload currentObjectPayload;
		payload.DeserializeNextObject(currentObjectPayload);

		CNetObject* objPtr = ObjectFactory()->MakeObject(currentObjectPayload.objectTypeID);
		ObjectFactory()->PreSpawn(*objPtr);
		_activeObjects.InsertWithID(objPtr, currentObjectPayload.objectID);

		SNetObjectHandle& spawnedHandle = _activeObjects.GetObject(currentObjectPayload.objectID);
		payload.PreSpawnRead(objPtr);

		CNetObjectInitializer initializer(_netContext, spawnedHandle, currentObjectPayload.ownerMask);
		objPtr->OnNetworkSpawn(initializer);

		payload.PostSpawnRead(spawnedHandle);

		ObjectFactory()->PostSpawn(*objPtr);
	}

	if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateNetObjectCount(_activeObjects.NumObjects());
}

// ------------------------------------------------------------

void CClientObjectHandler::RunNetObjectUpdate()
{
	_activeObjects.ForEach([&](auto& h) { UpdateNetObject(h); });
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------