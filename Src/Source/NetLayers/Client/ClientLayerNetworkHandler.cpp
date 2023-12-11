#include "Src/Include/NetLayers/Client/ClientLayerNetworkHandler.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Messaging/NetMessage.h"

// ------------------------------------------------------------

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

CClientLayerNetworkHandler::CClientLayerNetworkHandler(SNetLayerContext& netContext)
	: CNetLayerNetworkHandler()
	, _netContext(netContext)
{
}

// ------------------------------------------------------------

void CClientLayerNetworkHandler::RecievePending(CNetMessageQueue& messageQueue)
{
	auto interfacePtr = _netContext.backend.interfacePtr;

	SteamNetworkingMessage_t* msgPtr;
	while (interfacePtr->ReceiveMessagesOnConnection(_netContext.backend.hConnection, &msgPtr, 1) > 0)
	{
		if (_netContext.analyzerPtr) _netContext.analyzerPtr->NetDataRecieve(msgPtr->m_cbSize);

		messageQueue.PushRecieved(SNetMessage(msgPtr->m_pData, (size_t)msgPtr->m_cbSize));
		msgPtr->Release();
	}
}

// ------------------------------------------------------------

void CClientLayerNetworkHandler::SendPending(CNetMessageQueue& messageQueue)
{
	auto interfacePtr = _netContext.backend.interfacePtr;

	while (messageQueue.QueuedSend() > 0)
	{
		SNetMessage msg = messageQueue.NextQueuedSend();
		msg.SetSender(_netContext.layer.layerNetworkID);
		const int sendFlag = msg.reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;

		interfacePtr->SendMessageToConnection(_netContext.backend.hConnection, msg.pData, (uint32)msg.nBytes, sendFlag, nullptr);

		if (_netContext.analyzerPtr) _netContext.analyzerPtr->NetDataSend((unsigned int)msg.nBytes);
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------