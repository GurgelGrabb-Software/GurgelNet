#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/INetObjectInitializer.h"

struct SNetLayerContext;

struct SNetObjectHandle;

class CNetObjectInitializer : public INetObjectInitializer
{
public:
	CNetObjectInitializer(SNetLayerContext& netContext, SNetObjectHandle& handle, ClientID ownerMask);

	bool IsServer() const override;
	ClientID GetLocalNetID() const override;
	ClientID GetOwnerMask() const override;

	void RegisterNetVar(CNetworkVariable& regVar) override;
	void RegisterNetFunc(CNetFuncHandle& func) override;
private:
	SNetLayerContext& _netContext;
	SNetObjectHandle& _objectHandle;
	ClientID _ownerMask;
	bool _isServer;
};