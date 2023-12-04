#pragma once
#include "GurgelNet/Messages/INetMessageProcessor.h"

// ------------------------------------------------------------

class CNetLayerServer;
struct SNetLayerContext;

// ------------------------------------------------------------

class CServerConnectMessageProcessor : public INetMessageProcessor
{
public:
	CServerConnectMessageProcessor(CNetLayerServer& serverLayer, SNetLayerContext& context);

	uint8_t ProcessedMessageCategory() const override;
	void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer) override;
private:
	void ProcessIDConfirmation(const SNetMessageHeader& header, INetMessageReader& reader);
	void ProcessLateJoinComplete(const SNetMessageHeader& header, INetMessageReader& reader);

	CNetLayerServer& _serverLayer;
	SNetLayerContext& _netContext;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------