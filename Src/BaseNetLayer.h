#pragma once
#include "GurgelNet/INetLayer.h"
#include "Src/Messages/NetMessageQueue.h"
#include "Src/Messages/InternalMessageProcessor.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <vector>


class CNetLayerBase : public INetLayer
{
public:
	CNetLayerBase();
	virtual ~CNetLayerBase() override;

	EConnectState CurrentState() const override;
	
	void AssignNetID(uint8_t id);
	uint8_t GetNetID() const override;

	void RegisterProcessor(INetMessageProcessor* processor) override final;

	virtual void Start();
	virtual void Shutdown();

	void Tick();
protected:
	void ChangeState(EConnectState newState);

	virtual void Recieve() = 0;
	virtual void Send() = 0;

	CNetMessageQueue _messageQueue;
	ISteamNetworkingSockets* _interfacePtr;

private:
	uint8_t _localID;
	EConnectState _currentState = EConnectState_Inactive;
	std::vector<INetMessageProcessor*> _messageProcessors;
	CInternalMessageProcessor _internalProcessor;
};