#pragma once

class CNetworkVariable;

class INetObjectInitializer
{
public:
	virtual void RegisterNetVar(CNetworkVariable& regVar) = 0;
};