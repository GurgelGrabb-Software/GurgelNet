#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/Objects/NetObjectList.h"

CNetObjectInitializer::CNetObjectInitializer(SNetObjectHandle& handle, bool isServer)
	: _objectHandle(handle)
	, _isServer(isServer)
{
}

void CNetObjectInitializer::RegisterNetVar(CNetworkVariable& regVar)
{
	_objectHandle.netVariables.RegisterVariable(regVar);
}
