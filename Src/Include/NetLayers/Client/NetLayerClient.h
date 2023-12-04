#pragma once
#include "Src/Include/NetLayers/NetLayerShared.h"

#include "Src/Include/NetLayers/Client/ClientLayerConnectionHandler.h"
#include "Src/Include/NetLayers/Client/ClientLayerNetworkHandler.h"
#include "Src/Include/Objects/ClientObjectHandler.h"

// ------------------------------------------------------------

struct SteamNetConnectionStatusChangedCallback_t;

// ------------------------------------------------------------

class CNetLayerClient : public CNetLayerShared
{
public:
	CNetLayerClient();

	void Start() override;
	void Shutdown() override;

	CClientLayerConnectionHandler& ConnectionHandler();
	CClientObjectHandler& ObjectHandler();

	void ChangeClientState(EConnectState state);

private:
	static CNetLayerClient* s_instance;
	static void ClientConnectionCallback(SteamNetConnectionStatusChangedCallback_t* info) { s_instance->ConnectCallback(info); }
	void ConnectCallback(SteamNetConnectionStatusChangedCallback_t* info) override;

	CClientLayerNetworkHandler _netHandler;
	CClientLayerConnectionHandler _connectionHandler;
	CClientObjectHandler _objectHandler;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------