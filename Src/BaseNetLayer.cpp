#include "BaseNetLayer.h"
#include "Src/Serialization/NetMessageReader.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Messages/INetMessageProcessor.h"
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Core/Logging.h"

CNetLayerBase::CNetLayerBase()
	: _messageQueue()
	, _interfacePtr(nullptr)
	, _localID(0)
	, _currentState(EConnectState_Inactive)
	, _messageProcessors()
	, _internalProcessor(*this)
{
}

CNetLayerBase::~CNetLayerBase()
{
	if (CurrentState() != EConnectState_Inactive)
	{
		Shutdown();
	}

	for (auto processorPtr : _messageProcessors)
	{
		delete processorPtr;
	}
	_messageProcessors.clear();
}

EConnectState CNetLayerBase::CurrentState() const
{
	return _currentState;
}

uint8_t CNetLayerBase::GetNetID() const
{
	return _localID;
}

void CNetLayerBase::RegisterProcessor(INetMessageProcessor* processor)
{
	if (processor->ProcessedMessageCategory() == ENetMsgCategory_Internal)
	{
		NET_LOG(ENetLogLevel_Error, "Tried registering a message processor of category 0. This category is reserved for internal messages only.");
		return;
	}
	_messageProcessors.push_back(processor);
}

void CNetLayerBase::Start()
{
	_interfacePtr = SteamNetworkingSockets();
}

void CNetLayerBase::Shutdown()
{
}

void CNetLayerBase::Tick()
{
	Recieve();

	while (_messageQueue.QueuedRecieved() > 0)
	{
		SNetMessage message = _messageQueue.NextQueuedRecieved();

		CNetMessageReader reader(std::move(message));
		INetMessageReader& readerInterface = reader;

		SNetMessageHeader header;
		readerInterface.Read(header);

		if (header.categoryID == ENetMsgCategory_Internal)
		{
			_internalProcessor.Process(header, reader, _messageQueue);
		}
		else
		{
			for (auto processorPtr : _messageProcessors)
			{
				if (processorPtr->ProcessedMessageCategory() == header.categoryID)
				{
					processorPtr->Process(header, reader, _messageQueue);
					break;
				}
			}
		}
	}

	Send();
}

void CNetLayerBase::AssignNetID(uint8_t id)
{
	NET_LOG(ENetLogLevel_Verbose, "| NET LAYER | Local ID assigned: {}", id);
	_localID = id;
}

void CNetLayerBase::ChangeState(EConnectState newState)
{
	auto oldState = _currentState;
	_currentState = newState;

	if (oldState != EConnectState_Inactive && _currentState == EConnectState_Inactive)
	{
		Shutdown();
	}
}
