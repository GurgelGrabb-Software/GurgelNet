#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/NetworkFunction.h"
#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/NetworkFunctions/NetFuncCaller.h"

// ------------------------------------------------------------

class INetMessageQueue;

// ------------------------------------------------------------

class CNetFuncList
{
public:
	CNetFuncList(SNetLayerContext& netContext, NetObjectID objectID);
	CNetFuncList(const CNetFuncList&) = delete;
	CNetFuncList(CNetFuncList&& o) noexcept;

	ClientID GetLocalID() const;
	INetMessageQueue& GetQueue();

	void InvokeFunction(NetFuncID id, INetMessageReader& reader);

	void RegisterFunction(CNetFuncHandle& fHandle);
private:
	SNetLayerContext& _netContext;
	CNetFuncCaller _caller;
	std::vector<CNetFuncHandle*> _registeredFunctions;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------