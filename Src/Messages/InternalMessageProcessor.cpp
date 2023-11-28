#include "InternalMessageProcessor.h"
#include "Src/BaseNetLayer.h"
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Messages/InternalMessageTypes.h"
#include "GurgelNet/Messages/NetMessageHeader.h"
#include "GurgelNet/Serialization/INetSerializer.h"

CInternalMessageProcessor::CInternalMessageProcessor(CNetLayerBase& layer)
	: _layer(layer)
{
}

uint8_t CInternalMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Internal;
}

void CInternalMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetMessageQueue& messageQueue)
{
	switch (header.subTypeID)
	{
	case EInternalMsgT_AssignedID: 
	{
		uint8_t assignedID;
		reader.Read(assignedID);
		_layer.AssignNetID(assignedID);
	}
	break;

	default:
		break;
	}
}
