#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/INetObjectInitializer.h"

struct SNetObjectHandle;

class CNetObjectInitializer : public INetObjectInitializer
{
public:
	CNetObjectInitializer(SNetObjectHandle& handle, bool isServer);

	bool IsServer() const override { return _isServer; }

	void RegisterNetVar(CNetworkVariable& regVar) override;
private:
	SNetObjectHandle& _objectHandle;
	bool _isServer;
};