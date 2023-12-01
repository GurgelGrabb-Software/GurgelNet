#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/INetObjectInitializer.h"

class CNetObjectList;

class CNetObjectInitializer : public INetObjectInitializer
{
public:
	CNetObjectInitializer(NetObjectID ownerObjectID, CNetObjectList& objectList);
	void RegisterNetVar(CNetworkVariable& regVar) override;
private:
	NetObjectID _objectID;
	CNetObjectList& _objectList;
};