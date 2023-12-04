#pragma once

#include "GurgelNet/Messages/INetMessageProcessor.h"

// ------------------------------------------------------------

class CClientObjectHandler;

// ------------------------------------------------------------

class CClientObjectMessageProcessor : public INetMessageProcessor
{
public:
	CClientObjectMessageProcessor(CClientObjectHandler& objectHandler);

	uint8_t ProcessedMessageCategory() const override;
	void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer) override;

private:
	void ProcessObjectSpawnConfirm( INetMessageReader& reader );
	void ProcessObjectSpawn(INetMessageReader& reader);
	void ProcessNetVarSync(INetMessageReader& reader);

	CClientObjectHandler& _objectHandler;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------