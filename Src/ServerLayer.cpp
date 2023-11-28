#include "ServerLayer.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "Src/Core/Logging.h"

CServerLayer* CServerLayer::s_instancePtr = nullptr;

void ServerConnectCallback(SteamNetConnectionStatusChangedCallback_t* info)
{
	switch (info->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_Connecting:
		CServerLayer::s_instancePtr->Connecting(info->m_hConn);
		break;

	case k_ESteamNetworkingConnectionState_Connected:
		CServerLayer::s_instancePtr->Connected(info->m_hConn);
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		CServerLayer::s_instancePtr->Disconnected(info->m_hConn);
		break;
	}
}

CServerLayer::CServerLayer(unsigned short port)
	: _port(port)
	, _connection(0u)
	, _pollGroup(0u)
{
	// Server ID is always 1, remaining IDs are reserved for clients
	_freeClientIDs = _freeClientIDs & ~(1);
	AssignNetID(1);
	_messageQueue.AssignLocalID(1);

	s_instancePtr = this;
}

void CServerLayer::Start()
{
	NET_LOG(ENetLogLevel_Verbose, "Starting Server");

	CNetLayerBase::Start();

	SteamNetworkingIPAddr ip;
	ip.Clear();
	ip.m_port = _port;

	SteamNetworkingConfigValue_t cfg;
	cfg.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, ServerConnectCallback);

	_connection = _interfacePtr->CreateListenSocketIP(ip, 1, &cfg);
	_pollGroup = _interfacePtr->CreatePollGroup();
}

void CServerLayer::Shutdown()
{
	_interfacePtr->CloseListenSocket(_connection);
	_connection = 0;
	_interfacePtr->DestroyPollGroup(_pollGroup);
	_pollGroup = 0;
}

void CServerLayer::Recieve()
{
	SteamNetworkingMessage_t* msg;

	while (_interfacePtr->ReceiveMessagesOnPollGroup(_pollGroup, &msg, 1) > 0)
	{
		SNetMessage received = SNetMessage(msg->m_pData, msg->m_cbSize);

		const bool shouldRecieve = received.ShouldSendTo(1);
		const bool shouldForward = received.ShouldSendTo(0xFF ^ 1);

		if (shouldForward)
		{
			SendMessage(received, k_nSteamNetworkingSend_Unreliable); // TODO: We need to track if this message was originally sent as reliable and forward as such
		}
		if (shouldRecieve)
		{
			_messageQueue.PushRecieved(std::move(received));
		}
		msg->Release();
	}
}

void CServerLayer::Send()
{
	while (_messageQueue.QueuedSend() > 0)
	{
		SNetMessage msg = _messageQueue.NextQueuedSend();
		int sendFlag = k_nSteamNetworkingSend_Unreliable;
		if (msg.reliable) sendFlag = k_nSteamNetworkingSend_Reliable;

		if (msg.UseDefaultTarget()) msg.SetTarget(0xFF); // Default target for server is all clients

		SendMessage(msg, sendFlag);
	}
}

void CServerLayer::Connecting(unsigned int connectionID)
{
	if (connectionID == _connection)
	{
		ChangeState(EConnectState_PendingConnect);
	}
	else
	{
		NET_LOG(ENetLogLevel_Verbose, "| SERVER | New client connecting ({}), attempting to establish connection", connectionID);

		const uint8_t foundID = FindFreeClientID();
		if (foundID != 0)
		{
			NET_LOG(ENetLogLevel_Verbose, "| SERVER | Available client ID found: {}. Assigning and accepting.", foundID);
			AcceptClientConnection(foundID, connectionID);
			_interfacePtr->SetConnectionPollGroup(connectionID, _pollGroup);
		}
		else
		{
			NET_LOG(ENetLogLevel_Warning, "| SERVER | Denying connection, server is at capacity.");
			CloseClientConnection(connectionID);
		}
	}
}

void CServerLayer::Connected(unsigned int connectionID)
{
	if (connectionID == _connection)
	{
		ChangeState(EConnectState_Connected);
	}
	else
	{
		auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [&](const auto& ch) { return ch.connectionID == connectionID; });
		NET_LOG(ENetLogLevel_Verbose, "| SERVER | Connection established with client '{}', passing on assigned client ID '{}'", connectionID, it->clientID);
		_messageQueue.Send(SAssignedIDMsg(it->clientID), it->clientID, true);
	}
}

void CServerLayer::Disconnected(unsigned int connectionID)
{
	if (connectionID == _connection)
	{
		ChangeState(EConnectState_Inactive);
	}
	else
	{
		CloseClientConnection(connectionID);
	}
}

void CServerLayer::SendMessage(const SNetMessage& message, int sendFlag)
{
	for (const auto& clientHandle : _clientHandles)
	{
		if (message.ShouldSendTo(clientHandle.clientID))
		{
			_interfacePtr->SendMessageToConnection(clientHandle.connectionID, message.pData, (uint32)message.nBytes, sendFlag, nullptr);
		}
	}
}

void CServerLayer::AcceptClientConnection(uint8_t id, unsigned int connectionID)
{
	_interfacePtr->AcceptConnection(connectionID);
	ReserveClientID(id, connectionID);
}

void CServerLayer::CloseClientConnection(unsigned int connectionID)
{
	uint8_t clientID = 0;
	auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [&](const SClientHandle& h) { return h.connectionID == connectionID; });
	if (it != _clientHandles.end())
	{
		clientID = it->clientID;
	}

	if (clientID != 0) FreeClientID(clientID);
	_interfacePtr->CloseConnection(connectionID, 0, "", false);
}

uint8_t CServerLayer::FindFreeClientID() const
{
	for (size_t i = 1; i < 8; ++i)
	{
		const uint8_t checkID = 1 << i;
		if ((_freeClientIDs & checkID) != 0)
		{
			return checkID;
		}
	}

	return 0;
}

void CServerLayer::FreeClientID(uint8_t id)
{
	auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [&](const SClientHandle& h) { return h.clientID == id; });
	if (it != _clientHandles.end())
	{
		_clientHandles.erase(it);
	}
	_freeClientIDs = _freeClientIDs | id;
}

void CServerLayer::ReserveClientID(uint8_t id, unsigned int conID)
{
	_freeClientIDs = _freeClientIDs & ~(id);
	_clientHandles.push_back(SClientHandle{ .connectionID = conID, .clientID = id });
}
