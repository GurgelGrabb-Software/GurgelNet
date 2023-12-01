#include "ServerLayer.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "Src/Core/Logging.h"
#include "Src/Messages/ServerInternalMessageProcessor.h"

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
	: CNetLayerBase(new CServerInternalMessageProcessor(*this))
	, _port(port)
	, _connection(0u)
	, _pollGroup(0u)
{
	_freeClientIDs = ClientID_AllClients;
	AssignNetID(ClientID_Server);

	s_instancePtr = this;
}

void CServerLayer::Start()
{
	NET_LOG(ENetLogLevel_Verbose, "|SERVER| Starting");

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

		const bool shouldRecieve = ClientMask_Contains(received.GetTargetMask(), ClientID_Server);
		const bool shouldForward = ClientMask_Contains(received.GetTargetMask(), ClientID_AllClients);

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

		if (msg.UseDefaultTarget()) msg.SetTarget(ClientID_AllClients);

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
		NET_LOG(ENetLogLevel_Verbose, "|SERVER| New client connecting ({}), attempting to establish connection", connectionID);

		const uint8_t foundID = FindFreeClientID();
		if (foundID != 0)
		{
			NET_LOG(ENetLogLevel_Verbose, "|SERVER| Available client ID found: {}. Assigning and accepting.", foundID);
			AcceptClientConnection(foundID, connectionID);
			_interfacePtr->SetConnectionPollGroup(connectionID, _pollGroup);
		}
		else
		{
			NET_LOG(ENetLogLevel_Warning, "|SERVER| Denying connection, server is at capacity.");
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
		NET_LOG(ENetLogLevel_Verbose, "|SERVER| Connection established with client '{}', passing on assigned client ID '{}'", connectionID, it->clientID);
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

void CServerLayer::ApproveClientConnection(uint8_t clientID, bool approved)
{
	if (!approved)
	{
		auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [clientID](const SClientHandle& h) { return h.clientID == clientID; });
		NET_LOG(ENetLogLevel_Message, "|SERVER| Client with ID {} was not approved for connection. Terminating connection.", clientID);
		CloseClientConnection(it->connectionID);
		return;
	}

	void* lateSyncFuncPtr = nullptr;
	if (TryGetCallbackPtr(ENetLayerCallback_ServerSyncLateClient, &lateSyncFuncPtr))
	{
		// We need to do a late sync before connection is established
		CInternalMsg_Server_LateJoinSync syncMsg;
		syncMsg.serverWrite = (FNetWrite)lateSyncFuncPtr;
		_messageQueue.Send(syncMsg, clientID, true);
	}
	else
	{
		FinalizeApprovedClientConnection(clientID);
	}
}

void CServerLayer::SendMessage(const SNetMessage& message, int sendFlag)
{
	for (const auto& clientHandle : _clientHandles)
	{
		const bool clientIsTarget = ClientMask_Contains(message.GetTargetMask(), clientHandle.clientID);
		const bool clientIsFinalized = clientHandle.finalized;
		const bool messageIsInternal = message.GetCategory() == ENetMsgCategory_Internal;

		if (clientIsTarget && ( clientIsFinalized || messageIsInternal ))
		{
			_interfacePtr->SendMessageToConnection(clientHandle.connectionID, message.pData, (uint32)message.nBytes, sendFlag, nullptr);
		}
	}
}

void CServerLayer::FinalizeApprovedClientConnection(uint8_t clientID)
{
	auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [clientID](const auto& h) { return h.clientID == clientID; });
	it->finalized = true;

	void* connectedCallback = nullptr;
	if (TryGetCallbackPtr(ENetLayerCallback_ClientConnect, &connectedCallback))
	{
		((FClientConnection)connectedCallback)(clientID);
	}

	_messageQueue.Send(SHeaderOnlyMsg(EInternalMsg_ServerToClient_NotifyFinalized), clientID, true);
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

	NET_LOG(ENetLogLevel_Verbose, "|SERVER| ClientID {} (connection ID {}) disconnecting.", clientID, connectionID);


	_interfacePtr->CloseConnection(connectionID, 0, "", false);

	if (clientID != 0) 
	{
		FreeClientID(clientID);
	}
}

ClientID CServerLayer::FindFreeClientID() const
{
	for (size_t i = 1; i < 8; ++i)
	{
		const ClientID checkID = 1 << i;
		if ((_freeClientIDs & checkID) != 0)
		{
			return checkID;
		}
	}

	return 0;
}

void CServerLayer::FreeClientID(ClientID id)
{
	auto it = std::find_if(_clientHandles.begin(), _clientHandles.end(), [&](const SClientHandle& h) { return h.clientID == id; });
	if (it != _clientHandles.end())
	{
		_clientHandles.erase(it);
	}
	_freeClientIDs = ClientMask_Add(_freeClientIDs, id);
}

void CServerLayer::ReserveClientID(ClientID id, unsigned int conID)
{
	_freeClientIDs = ClientMask_Remove(_freeClientIDs, id);
	_clientHandles.push_back(SClientHandle{ .connectionID = conID, .clientID = id, .finalized = false });
}
