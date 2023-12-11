#pragma once
#include "GurgelNet/Messages/INetMessageQueue.h"
#include "Src/Include/Messaging/NetMessage.h"
#include "GurgelNet/Core/NetTypes.h"
#include <queue>

struct SNetLayerContext;

class CNetMessageQueue : public INetMessageQueue
{
public:
	void Send(const INetMessage& customMessage, bool reliable = false) override;
	void Send(const INetMessage& customMessage, ClientID targetMask, bool reliable = false) override;

	void Send(SNetMessage&& packagedMessage, bool reliable = false);
	void Send(SNetMessage&& packagedMessage, ClientID targetMask, bool reliable = false);

	void PushRecieved(SNetMessage&& msg);

	size_t QueuedSend() const;
	size_t QueuedRecieved() const;

	SNetMessage NextQueuedSend();
	SNetMessage NextQueuedRecieved();

	void AssignLocalID(ClientID id);

private:
	ClientID GetLocalID() const;
	ClientID _localID = ClientID_None;

	std::queue<SNetMessage> _sendQueue;
	std::queue<SNetMessage> _recieveQueue;
};