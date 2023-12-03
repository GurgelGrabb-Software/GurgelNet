#include "Src/Include/NetLayers/NetLayerShared.h"

// ------------------------------------------------------------

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

CNetLayerShared::CNetLayerShared(CNetLayerNetworkHandler& netHandler)
	: _ip("")
	, _port(0)
	, _netContext()
	, _messageQueue()
	, _callbackStore()
	, _netHandler(netHandler)
	, _state(EConnectState_Inactive)
{
	_netContext.layer.callbackStorePtr = &_callbackStore;
	_netContext.layer.msgQueuePtr = &_messageQueue;
}

// ------------------------------------------------------------

EConnectState CNetLayerShared::CurrentState() const
{
	return _state;
}

// ------------------------------------------------------------

ClientID CNetLayerShared::GetNetID() const
{
	return _netContext.layer.layerNetworkID;
}

// ------------------------------------------------------------

void CNetLayerShared::RegisterProcessor(INetMessageProcessor* processor)
{
	_netHandler.AddProcessor(*processor);
}

// ------------------------------------------------------------

INetMessageQueue& CNetLayerShared::MessageQueue()
{
	return _messageQueue;
}

// ------------------------------------------------------------

void CNetLayerShared::Initialize(SNetLayerSettings& settings)
{
	// Store IP and Port for later startup
	_ip = settings.ip;
	_port = settings.port;
	
	// Initialize all sub-parts
	_callbackStore.RegisterCallbacks(settings.layerCallbacks);

	// Bind the interface of the net context
	_netContext.backend.interfacePtr = SteamNetworkingSockets();
}

// ------------------------------------------------------------

void CNetLayerShared::Deinitialize()
{
}

// ------------------------------------------------------------

void CNetLayerShared::Update()
{
	_netHandler.RecievePending(_messageQueue);

	_netHandler.ProcessRecieved(_messageQueue, *this);

	_netHandler.SendPending(_messageQueue);
}

// ------------------------------------------------------------

void CNetLayerShared::ChangeState(EConnectState state)
{
	const EConnectState oldState = _state;
	_state = state;

	if (oldState == state) return; // Do nothing if there is no change

	FBasicCallback connectCallback = _callbackStore.GetCallbackAs<FBasicCallback>(ENetLayerCallback_LocalConnected);
	FBasicCallback disconnectCallback = _callbackStore.GetCallbackAs<FBasicCallback>(ENetLayerCallback_LocalDisconnected);

	if (_state == EConnectState_Connected && connectCallback) connectCallback();
	else if (_state == EConnectState_Inactive && disconnectCallback) disconnectCallback();
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------