#include "Src/Include/NetLayerMessages/ServerConnectionMessageProcessor.h"
#include "Src/Include/NetLayers/Server/NetLayerServer.h"

#include "Src/Include/NetLayerMessages/ConnectionMessages.h"

#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Messages/NetMessageTypes.h"

// ------------------------------------------------------------

CServerConnectMessageProcessor::CServerConnectMessageProcessor(CNetLayerServer& serverLayer, SNetLayerContext& context)
	: _serverLayer(serverLayer)
	, _netContext(context)
{
}

// ------------------------------------------------------------

uint8_t CServerConnectMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Connection;
}

// ------------------------------------------------------------

void CServerConnectMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EConnectMsg_ConfirmClientID: ProcessIDConfirmation(header, reader); break;
	case EConnectMsg_LateJoinComplete: ProcessLateJoinComplete(header, reader); break;
	default:
		break;
	}
}

// ------------------------------------------------------------

void CServerConnectMessageProcessor::ProcessIDConfirmation(const SNetMessageHeader& header, INetMessageReader& reader)
{
	_serverLayer.ConnectionsHandler().RunClientApproval(header.senderID, reader);
}

// ------------------------------------------------------------

void CServerConnectMessageProcessor::ProcessLateJoinComplete(const SNetMessageHeader& header, INetMessageReader& reader)
{
	_serverLayer.ConnectionsHandler().LateJoinCompleted(header.senderID);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------