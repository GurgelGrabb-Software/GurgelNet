#pragma once
#include "Src/Include/Objects/SharedObjectHandler.h"

#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/NetObjectList.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

#include <map>

// ------------------------------------------------------------

class CLateJoinPayload;

// ------------------------------------------------------------

class CServerObjectHandler : public CSharedObjectHandler
{
public:
	CServerObjectHandler(SNetLayerContext& netContext);

	void SpawnObject(CNetObject& object, ENetObjectOwner owner) override;
	void DespawnObject(CNetObject& object) override;
	void DespawnAllClientObjects(ClientID id);

	void ProcessObjectSpawnRequest(ClientID requestingClient, CObjectMsg_SpawnRequest& requestMsg);
	void ProcessObjectDespawn(ClientID requestingClient, CObjectMsg_Despawn& despawnMsg);
	void ProcessNetVarSync(CObjectMsg_NetVarSync& syncMsg);
	void ProcessNetFuncCall(CObjectMsg_NetFuncCall& callMsg);

	void WriteLateJoinPayload(CLateJoinPayload& payload);

	void RunNetObjectUpdate() override;

private:
	void DespawnInternal(CNetObject& object, ClientID expectedOwner);

	CNetObjectList _objects;
	std::map<ClientID, std::vector<NetObjectID>> _clientOwnedObjectIDs;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------