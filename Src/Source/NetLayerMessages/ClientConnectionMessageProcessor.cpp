#include "Src/Include/NetLayerMessages/ClientConnectionMessageProcessor.h"
#include "Src/Include/NetLayers/Client/NetLayerClient.h"

#include "Src/Include/NetLayerMessages/ConnectionMessages.h"

#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Messages/NetMessageTypes.h"

CClientConnectMessageProcessor::CClientConnectMessageProcessor(CNetLayerClient& clientLayer, SNetLayerContext& context)
	: _clientLayer(clientLayer)
	, _netContext(context)
{
}

uint8_t CClientConnectMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Connection;
}

void CClientConnectMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EConnectMsg_AssignClientID: ProcessClientIDAssigned(reader); break;
	case EConnectState_FinalizingConnection: ProcessConnectionFinalized(reader); break;
	case EConnectMsg_ConnectionActive: ProcessConnectionFinalized(reader); break;
	}

}

void CClientConnectMessageProcessor::ProcessClientIDAssigned(INetMessageReader& reader)
{
	// Read and process the ID
	CConnectMsg_AssignID idMsg;
	reader.Read(idMsg);
	_clientLayer.ConnectionHandler().RecieveAssignedID(idMsg.id);
}

void CClientConnectMessageProcessor::ProcessLateJoinSync(INetMessageReader& reader)
{
	_clientLayer.ConnectionHandler().RecieveLateJoinData(reader);
}

void CClientConnectMessageProcessor::ProcessConnectionFinalized(INetMessageReader& reader)
{
	_clientLayer.ConnectionHandler().RecieveConnectionConfirmation();
}
