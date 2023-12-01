#pragma once
#include "GurgelNet/Messages/INetMessageProcessor.h"

class CServerLayer;

class CServerInternalMessageProcessor : public INetMessageProcessor
{
public:
	CServerInternalMessageProcessor(CServerLayer& layer);

	virtual uint8_t ProcessedMessageCategory() const;
	virtual void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer);

private:
	CServerLayer& _layer;
};