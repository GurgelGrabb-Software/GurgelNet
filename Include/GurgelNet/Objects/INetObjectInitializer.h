// GurgelNet - 2023
// ------------------------------------------------------------
// INetObjectInitializer.h
// ------------------------------------------------------------
// Interface that will be passed to Network Object 
// OnNetworkSpawn
// 
// Allows you to initialize all your network data when the net-
// layer has been set up for the given object
// ------------------------------------------------------------

#pragma once

#include <functional>

// ------------------------------------------------------------

class CNetworkVariable;
class IRpcInvoker;
class CNetFuncHandle;

// ------------------------------------------------------------

class INetObjectInitializer
{
public:
	virtual bool IsServer() const = 0;
	virtual ClientID GetLocalNetID() const = 0;
	virtual ClientID GetOwnerMask() const = 0;
	virtual void RegisterNetVar(CNetworkVariable& regVar) = 0;
	virtual void RegisterNetFunc(CNetFuncHandle& rpc) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------