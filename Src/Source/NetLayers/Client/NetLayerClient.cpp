#include "Src/Include/NetLayers/Client/NetLayerClient.h"
#include "Src/Include/NetLayerMessages/ClientConnectionMessageProcessor.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

CNetLayerClient* CNetLayerClient::s_instance = nullptr;

// ------------------------------------------------------------

CNetLayerClient::CNetLayerClient()
	: CNetLayerShared(_netHandler)
	, _netHandler(_netContext)
	, _connectionHandler(*this, _netContext)
{
	s_instance = this;
}

// ------------------------------------------------------------

void CNetLayerClient::Start()
{
	auto interfacePtr = _netContext.backend.interfacePtr;

	CClientConnectMessageProcessor* connectProcessor = new CClientConnectMessageProcessor(*this, _netContext);
	_netHandler.AddProcessor(*connectProcessor);

	SteamNetworkingIPAddr connectionIP;
	connectionIP.ParseString(_ip.c_str());
	connectionIP.m_port = _port;

	SteamNetworkingConfigValue_t callbackConfig;
	callbackConfig.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, ClientConnectionCallback);

	const unsigned int hConnection = interfacePtr->ConnectByIPAddress(connectionIP, 1, &callbackConfig);
	_netContext.backend.hConnection = hConnection;

	ChangeState(EConnectState_PendingConnect);
}

// ------------------------------------------------------------

void CNetLayerClient::Shutdown()
{
	if (CurrentState() != EConnectState_Inactive)
	{
		auto interfacePtr = _netContext.backend.interfacePtr;
		interfacePtr->CloseConnection(_netContext.backend.hConnection, 0, "", false);

		_netContext.backend.hConnection = 0;
	}

	ChangeState(EConnectState_Inactive);
}

CClientLayerConnectionHandler& CNetLayerClient::ConnectionHandler()
{
	return _connectionHandler;
}

void CNetLayerClient::ChangeClientState(EConnectState state)
{
	ChangeState(state);
}

// ------------------------------------------------------------

void CNetLayerClient::ConnectCallback(SteamNetConnectionStatusChangedCallback_t* info)
{
	switch (info->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_Connecting:
		ChangeState(EConnectState_PendingConnect);
		break;
	case k_ESteamNetworkingConnectionState_Connected:
		ChangeState(EConnectState_FinalizingConnection);
		break;
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		Shutdown();
		break;
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------