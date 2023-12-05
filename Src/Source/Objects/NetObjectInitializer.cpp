#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/Objects/NetObjectList.h"

CNetObjectInitializer::CNetObjectInitializer(ClientID localID, SNetObjectHandle& handle, ClientID ownerMask)
	: _objectHandle(handle)
	, _localNetID(localID)
	, _ownerMask(ownerMask)
	, _isServer(localID == ClientID_Server)
{
}

bool CNetObjectInitializer::IsServer() const
{
	return _isServer;
}

ClientID CNetObjectInitializer::GetLocalNetID() const
{
	return _localNetID;
}

ClientID CNetObjectInitializer::GetOwnerMask() const
{
	return _ownerMask;
}

void CNetObjectInitializer::RegisterNetVar(CNetworkVariable& regVar)
{
	if (regVar.GetOwnerMask() == ClientID_None)
	{
		regVar.SetOwner(_ownerMask);
	}

	_objectHandle.netVariables.RegisterVariable(regVar, ClientMask_Contains(_ownerMask, _localNetID));
}
