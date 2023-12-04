#include "Src/Include/NetLayers/Server/NetLayerServer.h"

#include "Src/Include/NetLayerMessages/ServerConnectionMessageProcessor.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

CNetLayerServer* CNetLayerServer::s_instance = nullptr;

// ------------------------------------------------------------

CNetLayerServer::CNetLayerServer()
	: CNetLayerShared(_netHandler, _objectHandler)
	, _connectionsHandler(*this, _netContext)
	, _netHandler(_connectionsHandler, _netContext)
	, _objectHandler(_netContext)
{
	s_instance = this;
	_netContext.layer.layerNetworkID = ClientID_Server;
}

// ------------------------------------------------------------

void CNetLayerServer::Start()
{
	// Start up the backend
	auto interfacePtr = _netContext.backend.interfacePtr;

	CServerConnectMessageProcessor* connectProcessor = new CServerConnectMessageProcessor(*this, _netContext);
	_netHandler.AddProcessor(*connectProcessor);

	SteamNetworkingIPAddr connectionIP;
	connectionIP.Clear();
	connectionIP.m_port = _port;

	SteamNetworkingConfigValue_t callbackConfig;
	callbackConfig.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, ServerConnectionCallback);

	const unsigned int hConnection = interfacePtr->CreateListenSocketIP(connectionIP, 1, &callbackConfig);
	const unsigned int hPollGroup = interfacePtr->CreatePollGroup();

	// Update the context
	_netContext.backend.hConnection = hConnection;
	_netContext.backend.hPollGroup = hPollGroup;

	ChangeState(EConnectState_Connected);
}

// ------------------------------------------------------------

void CNetLayerServer::Shutdown()
{
	if (CurrentState() != EConnectState_Inactive)
	{
		auto interfacePtr = _netContext.backend.interfacePtr;
		interfacePtr->DestroyPollGroup(_netContext.backend.hPollGroup);
		interfacePtr->CloseConnection(_netContext.backend.hConnection, 0, "", false);

		_netContext.backend.hConnection = 0;
		_netContext.backend.hPollGroup = 0;
	}

	ChangeState(EConnectState_Inactive);
}

// ------------------------------------------------------------

CServerLayerConnectionsHandler& CNetLayerServer::ConnectionsHandler()
{
	return _connectionsHandler;
}

// ------------------------------------------------------------

CServerObjectHandler& CNetLayerServer::ObjectHandler()
{
	return _objectHandler;
}

// ------------------------------------------------------------

void CNetLayerServer::ConnectCallback(SteamNetConnectionStatusChangedCallback_t* info)
{
	switch (info->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_Connecting:
		_connectionsHandler.IncomingConnection(info->m_hConn);
		break;
	case k_ESteamNetworkingConnectionState_Connected:
		_connectionsHandler.EstablishedConnection(info->m_hConn);
		break;
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		_connectionsHandler.ClosedConnection(info->m_hConn);
		break;
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------