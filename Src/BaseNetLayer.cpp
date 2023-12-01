#include "BaseNetLayer.h"
#include "Src/Serialization/NetMessageReader.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Messages/INetMessageProcessor.h"
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Core/Logging.h"

CNetLayerBase::CNetLayerBase(INetMessageProcessor* internalProcessor)
	: _messageQueue()
	, _interfacePtr(nullptr)
	, _localID(0)
	, _currentState(EConnectState_Inactive)
	, _messageProcessors()
	, _internalProcessor(internalProcessor)
{
	for (size_t i = 0; i < ENetLayerCallback_Count; ++i)
	{
		_layerCallbackPtrs[i] = nullptr;
	}
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

	if (_internalProcessor)
	{
		delete _internalProcessor;
		_internalProcessor = nullptr;
	}
}

EConnectState CNetLayerBase::CurrentState() const
{
	return _currentState;
}

ClientID CNetLayerBase::GetNetID() const
{
	return _localID;
}

void CNetLayerBase::RegisterProcessor(INetMessageProcessor* processor)
{
	if (processor->ProcessedMessageCategory() == ENetMsgCategory_Internal)
	{
		NET_LOG(ENetLogLevel_Error, "|NET LAYER| Tried registering a message processor of category 0. This category is reserved for internal messages only.");
		return;
	}
	_messageProcessors.push_back(processor);
}

void CNetLayerBase::SetLayerCallback(ENetLayerCallback t, void* fPtr)
{
	_layerCallbackPtrs[t] = fPtr;
}

INetMessageQueue& CNetLayerBase::MessageQueue()
{
	return _messageQueue;
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
			_internalProcessor->Process(header, reader, *this);
		}
		else
		{
			for (auto processorPtr : _messageProcessors)
			{
				if (processorPtr->ProcessedMessageCategory() == header.categoryID)
				{
					processorPtr->Process(header, reader, *this);
					break;
				}
			}
		}
	}

	Send();
}

void CNetLayerBase::AssignNetID(ClientID id)
{
	NET_LOG(ENetLogLevel_Message, "|NET LAYER| Local ID assigned: {}", id);
	_localID = id;
	_messageQueue.AssignLocalID(id);
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

bool CNetLayerBase::TryGetCallbackPtr(ENetLayerCallback callbackType, void** outRawPtr)
{
	void* callback = _layerCallbackPtrs[callbackType];
	*outRawPtr = callback;
	return callback != nullptr;
}
