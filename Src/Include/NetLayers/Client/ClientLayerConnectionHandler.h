#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Serialization/INetSerializer.h"

class CNetLayerClient;
struct SNetLayerContext;

class CClientLayerConnectionHandler
{
public:
	CClientLayerConnectionHandler(CNetLayerClient& clientLayer, SNetLayerContext& context);

	void RecieveAssignedID(ClientID assignedID);
	void RecieveLateJoinData(INetMessageReader& lateJoinReader);
	void RecieveConnectionConfirmation();

private:
	CNetLayerClient& _clientLayer;
	SNetLayerContext& _netContext;
};