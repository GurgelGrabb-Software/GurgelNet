#pragma once
#include "GurgelNet/Messages/INetMessageProcessor.h"

class CNetLayerBase;

class CInternalMessageProcessor : public INetMessageProcessor
{
public:
	CInternalMessageProcessor(CNetLayerBase& layer);

	virtual uint8_t ProcessedMessageCategory() const;
	virtual void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetMessageQueue& messageQueue);

private:
	CNetLayerBase& _layer;
};