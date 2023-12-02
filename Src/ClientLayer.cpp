#include "ClientLayer.h"

#include "Src/Messages/ClientInternalMessageProcessor.h"
#include "Src/Messages/ObjectMessageTypes.h"
#include "Src/Objects/NetObjectInitializer.h"

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

void CClientLayer::RunNetVarSync()
{
	_activeObjectList.SyncNetworkVariables(_messageQueue);
}

CNetworkVariable* CClientLayer::GetNetVar(NetObjectID objectID, NetVarID varID)
{
	return _activeObjectList.GetNetVar(objectID, varID);
}

void CClientLayer::RecieveMessages()
{
	SteamNetworkingMessage_t* msg;
	while (_interfacePtr->ReceiveMessagesOnConnection(_connection, &msg, 1) > 0)
	{
		SNetMessage recieved = SNetMessage(msg->m_pData, msg->m_cbSize);
		_messageQueue.PushRecieved(std::move(recieved));
		msg->Release();
	}
}

void CClientLayer::SendQueuedMessages()
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

void CClientLayer::Send(const INetMessage& message, bool reliable)
{
	_messageQueue.Send(message, ClientID_Server, reliable);
}

void CClientLayer::Send(const INetMessage& message, ClientID targetMask, bool reliable)
{
	_messageQueue.Send(message, targetMask, reliable);
}

void CClientLayer::SpawnNetworkObject(CNetObject& spawn)
{
	// This is a client so push this object as pending and then request server to allocate an actual ID for it
	const auto assignedPendingID = _pendingObjectList.Add(&spawn);
	spawn.SetNetObjectID(assignedPendingID);

	// Send the spawn request
	CInternalMsg_Object_ClientRequestSpawn objectSpawnRequest;
	objectSpawnRequest.ObjectTypeID = spawn.GetNetTypeID();
	objectSpawnRequest.PendingID = assignedPendingID;
	objectSpawnRequest.object = &spawn;

	_messageQueue.Send(objectSpawnRequest, true);
}

void CClientLayer::ProcessObjectSpawn(CNetObject& spawned, NetObjectID objID)
{
	_activeObjectList.AddWithID(&spawned, objID);
	spawned.SetNetObjectID(objID);
	spawned.MarkAsClient();

	CNetObjectInitializer initializer(objID, _activeObjectList);
	spawned.OnNetworkSpawn(initializer);
}

void CClientLayer::ConfirmNetworkObjectSpawn(NetObjectID pendingID, NetObjectID confirmedID)
{
	CNetObject* object = _pendingObjectList.Get(pendingID);
	_pendingObjectList.Remove(pendingID);
	_activeObjectList.AddWithID(object, confirmedID);
	object->SetNetObjectID(confirmedID);
	object->MarkAsClient();

	CNetObjectInitializer initializer(confirmedID, _activeObjectList);
	object->OnNetworkSpawn(initializer); // The object is only really spawned at this point 
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
