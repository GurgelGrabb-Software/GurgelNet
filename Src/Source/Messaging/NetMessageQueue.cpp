#include "Src/Include/Messaging/NetMessageQueue.h"
#include "GurgelNet/Serialization/INetSerializable.h"
#include "GurgelNet/Messages/INetMessage.h"
#include "Src/Include/Serialization/NetMessageWriter.h"
#include "Src/Include/NetLayers/NetLayerContext.h"

void CNetMessageQueue::Send(const INetMessage& customMessage, bool reliable)
{
	CNetMessageWriter writer;
	writer.Write(customMessage);
	SNetMessage msg;
	msg.Package(writer, customMessage.CategoryID(), customMessage.SubTypeID());
	Send(std::move(msg), reliable);
}

void CNetMessageQueue::Send(const INetMessage& customMessage, ClientID targetMask, bool reliable)
{
	CNetMessageWriter writer;
	writer.Write(customMessage);
	SNetMessage msg;
	msg.Package(writer, customMessage.CategoryID(), customMessage.SubTypeID());
	Send(std::move(msg), targetMask, reliable);
}

void CNetMessageQueue::Send(SNetMessage&& packagedMessage, bool reliable)
{
	packagedMessage.SetSender(GetLocalID());
	packagedMessage.SetTarget(0);
	packagedMessage.reliable = reliable;
	_sendQueue.emplace(std::move(packagedMessage));
}

void CNetMessageQueue::Send(SNetMessage&& packagedMessage, ClientID targetMask, bool reliable)
{
	packagedMessage.SetSender(GetLocalID());
	packagedMessage.SetTarget(targetMask);
	packagedMessage.reliable = reliable;
	_sendQueue.emplace(std::move(packagedMessage));
}

void CNetMessageQueue::PushRecieved(SNetMessage&& msg)
{
	_recieveQueue.emplace(std::move(msg));
}

size_t CNetMessageQueue::QueuedSend() const
{
	return _sendQueue.size();
}

size_t CNetMessageQueue::QueuedRecieved() const
{
	return _recieveQueue.size();
}

SNetMessage CNetMessageQueue::NextQueuedSend()
{
	SNetMessage msg = std::move(_sendQueue.front());
	_sendQueue.pop();
	return msg;
}

SNetMessage CNetMessageQueue::NextQueuedRecieved()
{
	SNetMessage msg = std::move(_recieveQueue.front());
	_recieveQueue.pop();
	return msg;
}

void CNetMessageQueue::AssignLocalID(ClientID id)
{
	_localID = id;
}

ClientID CNetMessageQueue::GetLocalID() const
{
	return _localID;
}
