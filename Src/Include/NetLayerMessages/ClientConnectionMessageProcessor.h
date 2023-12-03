#pragma once
#include "GurgelNet/Messages/INetMessageProcessor.h"

class CNetLayerClient;
struct SNetLayerContext;

class CClientConnectMessageProcessor : public INetMessageProcessor
{
public:
	CClientConnectMessageProcessor(CNetLayerClient& clientLayer, SNetLayerContext& context);

	uint8_t ProcessedMessageCategory() const override;
	void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer) override;

private:
	void ProcessClientIDAssigned(INetMessageReader& reader);
	void ProcessLateJoinSync(INetMessageReader& reader);
	void ProcessConnectionFinalized(INetMessageReader& reader);

	CNetLayerClient& _clientLayer;
	SNetLayerContext& _netContext;
};
