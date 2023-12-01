#include "ClientLayer.h"

#include "Src/Messages/ClientInternalMessageProcessor.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include "Src/Core/Logging.h"

CClientLayer* CClientLayer::s_instancePtr = nullptr;

void ClientConnectCallback(SteamNetConnectionStatusChangedCallback_t* info)
{
	switch (info->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_Connecting: 
		CClientLayer::s_instancePtr->Connecting(); 
		break;
	
	case k_ESteamNetworkingConnectionState_Connected: 
		CClientLayer::s_instancePtr->Connected(); 
		break;
	
	case k_ESteamNetworkingConnectionState_ClosedByPeer: 
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		CClientLayer::s_instancePtr->Disconnected(); 
		break;
	}
}

CClientLayer::CClientLayer(const std::string& ip, unsigned short port)
	: CNetLayerBase(new CClientInternalMessageProcessor(*this))
	, _ip(ip)
	, _port(port)
	, _connection(0u)
{
	s_instancePtr = this;
}

void CClientLayer::Start()
{
	NET_LOG(ENetLogLevel_Verbose, "|CLIENT| Starting");

	CNetLayerBase::Start();
		
	SteamNetworkingIPAddr ip;
	ip.ParseString(_ip.c_str());
	ip.m_port = _port;

	SteamNetworkingConfigValue_t cfg;
	cfg.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, ClientConnectCallback);

	_connection = _interfacePtr->ConnectByIPAddress(ip, 1, &cfg);
}

void CClientLayer::Shutdown()
{
	_interfacePtr->CloseConnection(_connection, 0, "", false);
	_connection = 0u;
}

void CClientLayer::Recieve()
{
	SteamNetworkingMessage_t* msg;
	while (_interfacePtr->ReceiveMessagesOnConnection(_connection, &msg, 1) > 0)
	{
		SNetMessage recieved = SNetMessage(msg->m_pData, msg->m_cbSize);
		_messageQueue.PushRecieved(std::move(recieved));
		msg->Release();
	}
}

void CClientLayer::Send()
{
	while (_messageQueue.QueuedSend() > 0)
	{
		SNetMessage msg = _messageQueue.NextQueuedSend();
		int sendFlag = k_nSteamNetworkingSend_Unreliable;
		if (msg.reliable) sendFlag = k_nSteamNetworkingSend_Reliable;
		
		if (msg.UseDefaultTarget()) msg.SetTarget(ClientID_Server);				// Default target for clients is the server
		else msg.SetTarget(ClientMask_Remove(msg.GetTargetMask(), GetNetID()));	// Else ensure that we're not trying to send this to ourselves

		_interfacePtr->SendMessageToConnection(_connection, msg.pData, (uint32)msg.nBytes, sendFlag, nullptr);
	}
}

void CClientLayer::Connecting()
{
	NET_LOG(ENetLogLevel_Verbose, "|CLIENT| Attempting to connect to server");
	ChangeState(EConnectState_PendingConnect);
}

void CClientLayer::Connected()
{
	NET_LOG(ENetLogLevel_Verbose, "|CLIENT| Connection established with server, starting handshake.");
	ChangeState(EConnectState_FinalizingConnection);
}

void CClientLayer::Disconnected()
{
	NET_LOG(ENetLogLevel_Message, "|CLIENT| Disconnected from server");
	ChangeState(EConnectState_Inactive);
}

void CClientLayer::ConnectionFinalized()
{
	NET_LOG(ENetLogLevel_Confirm, "|CLIENT| Handshake complete, connection is active.");
	ChangeState(EConnectState_Connected);
	NotifyClientConnection(GetNetID());
}

void CClientLayer::NotifyClientConnection(ClientID id)
{
	void* connectedCallbackPtr = nullptr;
	if (TryGetCallbackPtr(ENetLayerCallback_ClientConnect, &connectedCallbackPtr))
	{
		((FClientConnection)connectedCallbackPtr)(id);
	}
}
