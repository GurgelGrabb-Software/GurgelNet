#pragma once
#include "Src/Include/NetLayers/NetLayerShared.h"

#include "Src/Include/NetLayers/Server/ServerLayerConnectionsHandler.h"
#include "Src/Include/NetLayers/Server/ServerLayerNetworkHandler.h"
#include "Src/Include/Objects/ServerObjectHandler.h"

struct SteamNetConnectionStatusChangedCallback_t;

class CNetLayerServer : public CNetLayerShared
{
public:
	CNetLayerServer();

	void Start() override;
	void Shutdown() override;

	CServerLayerConnectionsHandler& ConnectionsHandler();
	CServerObjectHandler& ObjectHandler();

private:
	static CNetLayerServer* s_instance;
	static void ServerConnectionCallback(SteamNetConnectionStatusChangedCallback_t* callbackInfo) { s_instance->ConnectCallback(callbackInfo); }
	void ConnectCallback(SteamNetConnectionStatusChangedCallback_t* info) override;

	CServerLayerConnectionsHandler _connectionsHandler;
	CServerLayerNetworkHandler _netHandler;
	CServerObjectHandler _objectHandler;
};