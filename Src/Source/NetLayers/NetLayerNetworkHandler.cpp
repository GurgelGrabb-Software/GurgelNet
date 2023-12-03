#include "Src/Include/NetLayers/NetLayerNetworkHandler.h"

#include "GurgelNet/Messages/INetMessageProcessor.h"
#include "GurgelNet/Messages/NetMessageHeader.h"

#include "Src/Include/Messaging/NetMessageQueue.h"
#include "Src/Include/Messaging/NetMessage.h"

#include "Src/Serialization/NetMessageReader.h"

// ------------------------------------------------------------

CNetLayerNetworkHandler::~CNetLayerNetworkHandler()
{
	for (auto [msgC, processorPtr] : _processors)
	{
		delete processorPtr;
	}

	_processors.clear();
}

// ------------------------------------------------------------

void CNetLayerNetworkHandler::AddProcessor(INetMessageProcessor& processor)
{
	_processors[processor.ProcessedMessageCategory()] = &processor;
}

// ------------------------------------------------------------

void CNetLayerNetworkHandler::ProcessRecieved(CNetMessageQueue& messageQueue, INetLayer& onLayer)
{
	while (messageQueue.QueuedRecieved() > 0)
	{
		SNetMessage msg = messageQueue.NextQueuedRecieved();
		
		// Create the reader
		CNetMessageReader reader(std::move(msg));
		INetMessageReader& iReader = reader;

		// Read the header
		SNetMessageHeader header;
		iReader.Read(header);

		// Process if found
		if (INetMessageProcessor* processor = FindProcessor(header.categoryID))
		{
			processor->Process(header, reader, onLayer);
		}
		else
		{
			// Warn the user that there is no processor registered for this category
		}
	}
}

// ------------------------------------------------------------

INetMessageProcessor* CNetLayerNetworkHandler::FindProcessor(uint8_t category)
{
	auto it = _processors.find(category);
	return it != _processors.end() ? it->second : nullptr;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------