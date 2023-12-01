#pragma once
#include "GurgelNet/Messages/INetMessageProcessor.h"

class CClientLayer;

class CClientInternalMessageProcessor : public INetMessageProcessor
{
public:
	CClientInternalMessageProcessor(CClientLayer& layer);

	virtual uint8_t ProcessedMessageCategory() const;
	virtual void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer);

private:
	CClientLayer& _layer;
};