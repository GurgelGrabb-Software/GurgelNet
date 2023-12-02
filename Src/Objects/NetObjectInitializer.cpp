#include "NetObjectInitializer.h"
#include "NetObjectList.h"

CNetObjectInitializer::CNetObjectInitializer(NetObjectID ownerObjectID, CNetObjectList& objectList)
	: _objectID(ownerObjectID)
	, _objectList(objectList)
{
}

void CNetObjectInitializer::RegisterNetVar(CNetworkVariable& regVar)
{
	_objectList.RegisterVariable(_objectID, regVar);
}
