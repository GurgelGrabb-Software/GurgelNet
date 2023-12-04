#pragma once
#include "Src/Include/Objects/SharedObjectHandler.h"

#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/NetObjectList.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

// ------------------------------------------------------------

class CLateJoinPayload;

// ------------------------------------------------------------

class CServerObjectHandler : public CSharedObjectHandler
{
public:
	CServerObjectHandler(SNetLayerContext& netContext);

	void SpawnObject(CNetObject& object) override;

	void ProcessObjectSpawnRequest(ClientID requestingClient, CObjectMsg_SpawnRequest& requestMsg);
	void ProcessNetVarSync(CObjectMsg_NetVarSync& syncMsg);

	void WriteLateJoinPayload(CLateJoinPayload& payload);

	void RunNetObjectUpdate() override;

private:
	CNetObjectList _objects;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------