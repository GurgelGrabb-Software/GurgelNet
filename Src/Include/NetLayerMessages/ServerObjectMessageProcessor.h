#pragma once

#include "GurgelNet/Messages/INetMessageProcessor.h"

// ------------------------------------------------------------

class CServerObjectHandler;

// ------------------------------------------------------------

class CServerObjectMessageProcessor : public INetMessageProcessor
{
public:
	CServerObjectMessageProcessor(CServerObjectHandler& objectHandler);

	uint8_t ProcessedMessageCategory() const override;
	void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer) override;

private:
	void ProcessSpawnRequest(const SNetMessageHeader& header, INetMessageReader& reader);
	void ProcessNetVarSync(INetMessageReader& reader);
	void ProcessNetFuncCall(INetMessageReader& reader);

	CServerObjectHandler& _objectHandler;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------