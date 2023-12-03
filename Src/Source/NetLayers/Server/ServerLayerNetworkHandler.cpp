#include "Src/Include/NetLayers/Server/ServerLayerNetworkHandler.h"

#include "Src/Include/Messaging/NetMessage.h"
#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/NetLayers/Server/ServerLayerConnectionsHandler.h"

#include "GurgelNet/Messages/NetMessageTypes.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

CServerLayerNetworkHandler::CServerLayerNetworkHandler(CServerLayerConnectionsHandler& connections, SNetLayerContext& netContext)
	: _connections(connections)
	, _netContext(netContext)
{
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::RecievePending(CNetMessageQueue& messageQueue)
{
	auto interfacePtr = _netContext.backend.interfacePtr;

	SteamNetworkingMessage_t* msgPtr;
	while (interfacePtr->ReceiveMessagesOnPollGroup(_netContext.backend.hPollGroup, &msgPtr, 1) > 0)
	{
		RecieveMessage(msgPtr->m_pData, msgPtr->m_cbSize, messageQueue);
		msgPtr->Release();
	}
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::SendPending(CNetMessageQueue& messageQueue)
{
	while (messageQueue.QueuedSend() > 0)
	{
		SNetMessage msg = messageQueue.NextQueuedSend();
		SendMessage(msg);
	}
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::RecieveMessage(void* data, unsigned int size, CNetMessageQueue& messageQueue)
{
	SNetMessage msg = SNetMessage(data, (size_t)size);
	const bool recieve = ClientMask_Contains(msg.GetTargetMask(), ClientID_Server);
	const bool forward = ClientMask_Remove(msg.GetTargetMask(), ClientID_Server) != ClientID_None;

	if (forward)
	{
		ForwardMessage(msg);
	}
	if (recieve)
	{
		messageQueue.PushRecieved(std::move(msg));
	}
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::SendMessage(SNetMessage& msg)
{
	const auto& connectionList = _connections.GetConnections().GetConnectionsList();
	msg.SetSender(_netContext.layer.layerNetworkID);

	const int sendFlag = msg.reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;

	for (auto& c : connectionList)
	{
		TrySendToConnection(msg, sendFlag, c);
	}
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::TrySendToConnection(SNetMessage& msg, int sendFlag, const SClientConnection& connection)
{
	const bool isLibraryMessage = msg.GetCategory() < ENetMsgCategory_Custom;
	const bool connectionTargeted = ClientMask_Contains(msg.GetTargetMask(), connection.clientID);
	const bool connectionTargetable = connection.active || isLibraryMessage;

	if (connectionTargeted && connectionTargetable)
	{
		_netContext.backend.interfacePtr->SendMessageToConnection(connection.hConnection, msg.pData, msg.nBytes, sendFlag, nullptr);
	}
}

// ------------------------------------------------------------

void CServerLayerNetworkHandler::ForwardMessage(SNetMessage& msg)
{
	auto originalTargetMask = msg.GetTargetMask();
	
	ClientID targetMask = originalTargetMask;
	targetMask = ClientMask_Remove(targetMask, ClientID_Server);	// Don't forward to server (we're here now)
	targetMask = ClientMask_Remove(targetMask, msg.GetSenderID());	// Don't forward back to original sender

	msg.SetTarget(targetMask); 

	SendMessage(msg);

	msg.SetTarget(originalTargetMask); // Reset the message back to its original state
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------