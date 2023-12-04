#include "Src/Include/Objects/ClientObjectHandler.h"

#include "GurgelNet/Objects/INetObjectFactory.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Objects/NetObjectInitializer.h"

#include "Src/Include/NetLayerMessages/LateJoinPayload.h"

// ------------------------------------------------------------

CClientObjectHandler::CClientObjectHandler(SNetLayerContext& netContext)
	: CSharedObjectHandler(netContext)
	, _pendingObjects()
	, _activeObjects()
{
}

// ------------------------------------------------------------

void CClientObjectHandler::SpawnObject(CNetObject& object)
{
	NetObjectID pendingID = _pendingObjects.Insert(&object);

	CObjectMsg_SpawnRequest spawnRequestMsg;
	spawnRequestMsg.objectHandle = &_pendingObjects.GetObject(pendingID);

	_netContext.layer.msgQueuePtr->Send(spawnRequestMsg, ClientID_Server, true);
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
	CNetObjectInitializer initializer(activeHandle, false);
	activeHandle.objectPtr->OnNetworkSpawn(initializer);
}

// ------------------------------------------------------------

void CClientObjectHandler::ProcessObjectSpawn(CObjectMsg_Spawn& spawnMsg)
{
	const NetObjectID objectID = spawnMsg.spawnedObjectID;
	const NetTypeID objectTypeID = spawnMsg.spawnedTypeID;

	// Insert to object list
	CNetObject* const object = ObjectFactory()->MakeObject(objectTypeID);
	_activeObjects.InsertWithID(object, objectID);

	// Read the object data (initialization data)
	SNetObjectHandle& activeHandle = _activeObjects.GetObject(objectID);
	spawnMsg.ReadPreSpawnData(activeHandle);

	// Run the spawn
	CNetObjectInitializer initializer(activeHandle, false);
	object->OnNetworkSpawn(initializer);

	// Read post spawn data (initial net var values)
	spawnMsg.ReadPostSpawnData(activeHandle);
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

void CClientObjectHandler::ProcessLateJoinPayload(CLateJoinPayload& payload)
{
	for (NetObjectID i = 0; i < payload.numObjects; ++i)
	{
		SLateJoinObjectPayload currentObjectPayload;
		payload.DeserializeNextObject(currentObjectPayload);

		CNetObject* objPtr = ObjectFactory()->MakeObject(currentObjectPayload.objectTypeID);
		_activeObjects.InsertWithID(objPtr, currentObjectPayload.objectID);

		SNetObjectHandle& spawnedHandle = _activeObjects.GetObject(currentObjectPayload.objectID);
		payload.PreSpawnRead(objPtr);

		CNetObjectInitializer initializer(spawnedHandle, false);
		objPtr->OnNetworkSpawn(initializer);

		payload.PostSpawnRead(spawnedHandle);
	}
}

// ------------------------------------------------------------

void CClientObjectHandler::RunNetObjectUpdate()
{
	_activeObjects.ForEach([&](auto& h) { UpdateNetObject(h); });
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------