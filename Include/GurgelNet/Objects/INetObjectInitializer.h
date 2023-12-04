#pragma once

class CNetworkVariable;

class INetObjectInitializer
{
public:
	virtual bool IsServer() const = 0;

	virtual void RegisterNetVar(CNetworkVariable& regVar) = 0;
};