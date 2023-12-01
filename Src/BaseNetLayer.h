#pragma once
#include "GurgelNet/INetLayer.h"
#include "Src/Messages/NetMessageQueue.h"
#include "Src/Messages/ClientInternalMessageProcessor.h"
#include "GurgelNet/Objects/INetObjectFactory.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <vector>


class CNetLayerBase : public INetLayer
{
public:
	CNetLayerBase(INetMessageProcessor* internalProcessor);
	virtual ~CNetLayerBase() override;

	EConnectState CurrentState() const override;

	void RegisterProcessor(INetMessageProcessor* processor) override final;
	void SetLayerCallback(ENetLayerCallback t, void* fPtr);
	
	INetMessageQueue& MessageQueue();

	INetObjectFactory& GetObjectFactory();

	void RegisterObjectFactory(INetObjectFactory& factory);

	void AssignNetID(ClientID id);
	ClientID GetNetID() const override;

	virtual void Start();
	virtual void Shutdown();
	void Tick();

	bool TryGetCallbackPtr(ENetLayerCallback callbackType, void** outRawPtr);
protected:
	void ChangeState(EConnectState newState);

	virtual void RecieveMessages() = 0;
	virtual void SendQueuedMessages() = 0;
	virtual void RunNetVarSync() = 0;

	CNetMessageQueue _messageQueue;
	ISteamNetworkingSockets* _interfacePtr;

protected:
	 INetMessageProcessor* _internalProcessor;

private:
	ClientID _localID;
	EConnectState _currentState = EConnectState_Inactive;

	INetObjectFactory* _objectFactory;
	
	std::vector<INetMessageProcessor*> _messageProcessors;
	void* _layerCallbackPtrs[ENetLayerCallback_Count];
	
};