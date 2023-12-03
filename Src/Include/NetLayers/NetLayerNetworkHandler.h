#pragma once
#include <unordered_map>
#include <cstdint>

struct SNetLayerContext;
class CNetMessageQueue;
class INetMessageProcessor;
class INetLayer;

class CNetLayerNetworkHandler
{
public:

	virtual ~CNetLayerNetworkHandler();

	virtual void RecievePending(CNetMessageQueue& messageQueue) = 0;
	virtual void SendPending(CNetMessageQueue& messageQueue) = 0;

	void AddProcessor(INetMessageProcessor& processor);
	void ProcessRecieved(CNetMessageQueue& messageQueue, INetLayer& onLayer);

private:
	INetMessageProcessor* FindProcessor(uint8_t category);
	std::unordered_map<uint8_t, INetMessageProcessor*> _processors;
};