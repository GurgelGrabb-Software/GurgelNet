#pragma once
#include "Src/Include/NetLayers/NetLayerNetworkHandler.h"
#include "Src/Include/NetLayers/NetLayerContext.h"

// ------------------------------------------------------------

class CClientLayerNetworkHandler : public CNetLayerNetworkHandler
{
public:
	CClientLayerNetworkHandler(SNetLayerContext& netContext);

	virtual void RecievePending(CNetMessageQueue& messageQueue) override;
	virtual void SendPending(CNetMessageQueue& messageQueue) override;
private:
	SNetLayerContext& _netContext;

};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------