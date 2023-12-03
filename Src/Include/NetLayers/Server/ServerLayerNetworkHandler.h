#pragma once
#include "Src/Include/NetLayers/NetLayerNetworkHandler.h"

#include "Src/Include/NetLayers/Server/ServerLayerConnectionsHandler.h"
#include "Src/Include/NetLayers/NetLayerContext.h"

// ------------------------------------------------------------

struct SNetMessage;
class CServerLayerConnectionsHandler;

// ------------------------------------------------------------

class CServerLayerNetworkHandler : public CNetLayerNetworkHandler
{
public:
	CServerLayerNetworkHandler(CServerLayerConnectionsHandler& connections, SNetLayerContext& netContext);
	
	void RecievePending(CNetMessageQueue& messageQueue) override;
	void SendPending(CNetMessageQueue& messageQueue) override;

private:
	void RecieveMessage(void* data, unsigned int size, CNetMessageQueue& messageQueue);
	void SendMessage(SNetMessage& msg);
	void TrySendToConnection(SNetMessage& msg, int sendFlag, const SClientConnection& connection);
	void ForwardMessage(SNetMessage& msg);

	const CServerLayerConnectionsHandler& _connections;
	SNetLayerContext& _netContext;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------