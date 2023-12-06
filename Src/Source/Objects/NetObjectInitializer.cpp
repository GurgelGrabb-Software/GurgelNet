#include "Src/Include/Objects/NetObjectInitializer.h"
#include "Src/Include/Objects/NetObjectList.h"
#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/NetworkFunctions/NetFuncList.h"

CNetObjectInitializer::CNetObjectInitializer(SNetLayerContext& netContext, SNetObjectHandle& handle, ClientID ownerMask)
	: _netContext(netContext)
	, _objectHandle(handle)
	, _ownerMask(ownerMask)
	, _isServer(netContext.layer.layerNetworkID == ClientID_Server)
{
	if (handle.netFuncList == nullptr)
	{
		handle.netFuncList = new CNetFuncList(_netContext, _objectHandle.objectPtr->GetNetObjectID());
	}
}

bool CNetObjectInitializer::IsServer() const
{
	return _isServer;
}

ClientID CNetObjectInitializer::GetLocalNetID() const
{
	return _netContext.layer.layerNetworkID;
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

	_objectHandle.netVariables.RegisterVariable(regVar, ClientMask_Contains(regVar.GetOwnerMask(), _netContext.layer.layerNetworkID));
}

void CNetObjectInitializer::RegisterNetFunc(CNetFuncHandle& func)
{
	_objectHandle.netFuncList->RegisterFunction(func);
}

