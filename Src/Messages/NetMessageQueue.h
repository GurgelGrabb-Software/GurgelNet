#pragma once
#include "GurgelNet/Messages/INetMessageQueue.h"
#include "Src/Messages/NetMessage.h"
#include <queue>

class CNetMessageQueue : public INetMessageQueue
{
public:
	void Send(const INetMessage& customMessage, bool reliable = false) override;
	void Send(const INetMessage& customMessage, uint8_t targetMask, bool reliable = false) override;

	void Send(SNetMessage&& packagedMessage, bool reliable = false);
	void Send(SNetMessage&& packagedMessage, uint8_t targetMask, bool reliable = false);

	void PushRecieved(SNetMessage&& msg);

	size_t QueuedSend() const;
	size_t QueuedRecieved() const;

	SNetMessage NextQueuedSend();
	SNetMessage NextQueuedRecieved();

	void AssignLocalID(uint8_t id);

private:
	uint8_t GetLocalID() const;
	uint8_t _localID;

	std::queue<SNetMessage> _sendQueue;
	std::queue<SNetMessage> _recieveQueue;
};