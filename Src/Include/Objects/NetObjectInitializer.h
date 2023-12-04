#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/INetObjectInitializer.h"

struct SNetObjectHandle;

class CNetObjectInitializer : public INetObjectInitializer
{
public:
	CNetObjectInitializer(SNetObjectHandle& handle);
	void RegisterNetVar(CNetworkVariable& regVar) override;
private:
	SNetObjectHandle& _objectHandle;
};