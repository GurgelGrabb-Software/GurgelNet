#include "Src/Include/Objects/SharedObjectHandler.h"

#include "Src/Include/Objects/NetObjectHandle.h"
#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

CSharedObjectHandler::CSharedObjectHandler(SNetLayerContext& netContext)
	: _netContext(netContext)
	, _factoryPtr(nullptr)
{
}

void CSharedObjectHandler::AssignObjectFactory(INetObjectFactory* factoryPtr)
{
	_factoryPtr = factoryPtr;
}

INetObjectFactory* CSharedObjectHandler::ObjectFactory()
{
	return _factoryPtr;
}

void CSharedObjectHandler::UpdateNetObject(SNetObjectHandle& handle)
{
	const NetVarID numNetVars = handle.netVariables.Num();

	for (NetVarID i = 0; i < numNetVars; ++i)
	{
		CNetworkVariable* varPtr = handle.netVariables.GetVariable(i);
		UpdateNetVar(handle.objectPtr->GetNetObjectID(), *varPtr);
	}
}

void CSharedObjectHandler::UpdateNetVar(NetObjectID objectID, CNetworkVariable& var)
{
	if (var.IsOwner(_netContext.layer.layerNetworkID) &&
		var.ShouldSync())
	{
		CObjectMsg_NetVarSync syncMsg;
		syncMsg.objectID = objectID;
		syncMsg.varPtr = &var;

		_netContext.layer.msgQueuePtr->Send(syncMsg, ClientID_AllExcept(_netContext.layer.layerNetworkID), true);
	}
}
