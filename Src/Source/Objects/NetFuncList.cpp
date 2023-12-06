#include "Src/Include/Objects/NetworkFunctions/NetFuncList.h"
#include "GurgelNet/INetLayer.h"

// ------------------------------------------------------------

CNetFuncList::CNetFuncList(SNetLayerContext& netContext, NetObjectID objectID)
	: _netContext(netContext)
	, _caller(*this, objectID)
	, _registeredFunctions()
{
}

// ------------------------------------------------------------

CNetFuncList::CNetFuncList(CNetFuncList&& o) noexcept
	: _netContext(o._netContext)
	, _caller(o._caller)
	, _registeredFunctions(std::move(o._registeredFunctions))
{
}

// ------------------------------------------------------------

ClientID CNetFuncList::GetLocalID() const
{
	return _netContext.layer.layerNetworkID;
}

// ------------------------------------------------------------

INetMessageQueue& CNetFuncList::GetQueue()
{
	return *_netContext.layer.msgQueuePtr;
}

// ------------------------------------------------------------

void CNetFuncList::InvokeFunction(NetFuncID id, INetMessageReader& reader)
{
	_registeredFunctions[id]->RunInvoke(reader);
}

// ------------------------------------------------------------

void CNetFuncList::RegisterFunction(CNetFuncHandle& fHandle)
{
	NetFuncID assignedID = (NetFuncID)_registeredFunctions.size();
	_registeredFunctions.push_back(&fHandle);

	fHandle.Initialize(assignedID, GetLocalID() == ClientID_Server, &_caller);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------