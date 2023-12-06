#include "Src/Include/NetLayerMessages/ServerObjectMessageProcessor.h"

#include "GurgelNet/Messages/NetMessageHeader.h"

#include "Src/Include/Objects/ServerObjectHandler.h"

#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Include/NetLayerMessages/NetLayerMessageTypes.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

// ------------------------------------------------------------

CServerObjectMessageProcessor::CServerObjectMessageProcessor(CServerObjectHandler& objectHandler)
	: _objectHandler(objectHandler)
{
}

// ------------------------------------------------------------

uint8_t CServerObjectMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Object;
}

// ------------------------------------------------------------

void CServerObjectMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EObjectMsg_SpawnRequest: ProcessSpawnRequest(header, reader); break;
	case EObjectMsg_NetVarSync: ProcessNetVarSync(reader); break;
	case EObjectMsg_NetFuncCall: ProcessNetFuncCall(reader); break;
	}
}

// ------------------------------------------------------------

void CServerObjectMessageProcessor::ProcessSpawnRequest(const SNetMessageHeader& header, INetMessageReader& reader)
{
	CObjectMsg_SpawnRequest requestMsg;
	reader.Read(requestMsg);
	_objectHandler.ProcessObjectSpawnRequest(header.senderID, requestMsg);
}

// ------------------------------------------------------------

void CServerObjectMessageProcessor::ProcessNetVarSync(INetMessageReader& reader)
{
	CObjectMsg_NetVarSync syncMsg;
	reader.Read(syncMsg);
	_objectHandler.ProcessNetVarSync(syncMsg);
}

// ------------------------------------------------------------

void CServerObjectMessageProcessor::ProcessNetFuncCall(INetMessageReader& reader)
{
	CObjectMsg_NetFuncCall funcCall;
	reader.Read(funcCall);
	_objectHandler.ProcessNetFuncCall(funcCall);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------