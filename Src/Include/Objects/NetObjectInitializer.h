#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/INetObjectInitializer.h"

struct SNetObjectHandle;

class CNetObjectInitializer : public INetObjectInitializer
{
public:
	CNetObjectInitializer(ClientID localID, SNetObjectHandle& handle, ClientID ownerMask);

	bool IsServer() const override;
	ClientID GetLocalNetID() const override;
	ClientID GetOwnerMask() const override;


	void RegisterNetVar(CNetworkVariable& regVar) override;
private:
	SNetObjectHandle& _objectHandle;
	ClientID _localNetID;
	ClientID _ownerMask;
	bool _isServer;
};