#pragma once
#include "Src/Include/Objects/SharedObjectHandler.h"

#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/NetObjectList.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

#include "GurgelNet/Objects/NetObjectOwnership.h"

class CLateJoinPayload;

class CClientObjectHandler : public CSharedObjectHandler
{
public:
	CClientObjectHandler(SNetLayerContext& netContext);

	void SpawnObject(CNetObject& object, ENetObjectOwner owner) override;
	void DespawnObject(CNetObject& object) override;

	void ObjectSpawnConfirmed(CObjectMsg_SpawnConfirm& confirmMsg);
	void ProcessObjectSpawn(CObjectMsg_Spawn& spawnMsg);
	void ProcessObjectDespawn(CObjectMsg_Despawn& despawnMsg);
	void SyncNetVar(CObjectMsg_NetVarSync& syncMsg);
	void ProcessNetFuncCall(CObjectMsg_NetFuncCall& callMsg);

	void ProcessLateJoinPayload(CLateJoinPayload& payload);

	void RunNetObjectUpdate() override;

private:
	CNetObjectList _pendingObjects;
	CNetObjectList _activeObjects;
};