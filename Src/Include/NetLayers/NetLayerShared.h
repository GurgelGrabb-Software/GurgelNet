#pragma once

#include "GurgelNet/INetLayer.h"

#include "GurgelNet/Core/LayerSettings.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/NetLayers/NetLayerCallbackStore.h"
#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/SharedObjectHandler.h"

#include "Src/Include/NetLayers/NetLayerNetworkHandler.h"

struct SteamNetConnectionStatusChangedCallback_t;
class INetObjectFactory;

class CNetLayerShared : public INetLayer
{
public:

	CNetLayerShared(CNetLayerNetworkHandler& netHandler, CSharedObjectHandler& objectHandler);
	virtual ~CNetLayerShared() = default;

	// ------------------------------------------------------------
	// INetLayer
	// ------------------------------------------------------------
	EConnectState CurrentState() const override;
	ClientID GetNetID() const override;
	void RegisterProcessor(INetMessageProcessor* processor) override;
	INetMessageQueue& MessageQueue() override;

	virtual void SpawnNetworkObject(CNetObject& spawn);

	// ------------------------------------------------------------
	// Execution Flow
	// ------------------------------------------------------------

	virtual void Initialize(SNetLayerSettings& settings);
	virtual void Deinitialize();

	virtual void Start() = 0;
	virtual void Shutdown() = 0;

	virtual void Update();

protected:
	virtual void ConnectCallback(SteamNetConnectionStatusChangedCallback_t* info) = 0;

	void ChangeState(EConnectState state);

	std::string _ip;
	unsigned short _port;

	SNetLayerContext _netContext;
private:
	CNetMessageQueue _messageQueue;
	CNetLayerCallbackStore _callbackStore;
	CNetLayerNetworkHandler& _netHandler;
	CSharedObjectHandler& _objectHandler;

	EConnectState _state;
};
