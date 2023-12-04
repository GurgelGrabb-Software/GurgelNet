#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/Objects/NetObjectList.h"

CNetObjectInitializer::CNetObjectInitializer(SNetObjectHandle& handle)
	: _objectHandle(handle)
{
}

void CNetObjectInitializer::RegisterNetVar(CNetworkVariable& regVar)
{
	_objectHandle.netVariables.RegisterVariable(regVar);
}
