#include "Src/Include/NetLayerMessages/ClientObjectMessageProcessors.h"

#include "GurgelNet/Messages/NetMessageHeader.h"

#include "Src/Include/Objects/ClientObjectHandler.h"

#include "GurgelNet/Messages/NetMessageTypes.h"
#include "Src/Include/NetLayerMessages/NetLayerMessageTypes.h"
#include "Src/Include/NetLayerMessages/ObjectMessages.h"

// ------------------------------------------------------------

CClientObjectMessageProcessor::CClientObjectMessageProcessor(CClientObjectHandler& objectHandler)
	: _objectHandler(objectHandler)
{
}

// ------------------------------------------------------------

uint8_t CClientObjectMessageProcessor::ProcessedMessageCategory() const
{
	return ENetMsgCategory_Object;
}

// ------------------------------------------------------------

void CClientObjectMessageProcessor::Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer)
{
	switch (header.subTypeID)
	{
	case EObjectMsg_Spawn: ProcessObjectSpawn(reader); break;
	case EObjectMsg_SpawnConfirm: ProcessObjectSpawnConfirm(reader); break;
	case EObjectMsg_NetVarSync: ProcessNetVarSync(reader); break;
	case EObjectMsg_NetFuncCall: ProcessNetFuncCall(reader); break;
	}
}

// ------------------------------------------------------------

void CClientObjectMessageProcessor::ProcessObjectSpawnConfirm(INetMessageReader& reader)
{
	CObjectMsg_SpawnConfirm confirmMsg;
	reader.Read(confirmMsg);
	_objectHandler.ObjectSpawnConfirmed(confirmMsg);
}

// ------------------------------------------------------------

void CClientObjectMessageProcessor::ProcessObjectSpawn(INetMessageReader& reader)
{
	CObjectMsg_Spawn spawnMsg;
	reader.Read(spawnMsg);
	_objectHandler.ProcessObjectSpawn(spawnMsg);
}

// ------------------------------------------------------------

void CClientObjectMessageProcessor::ProcessNetVarSync(INetMessageReader& reader)
{
	CObjectMsg_NetVarSync syncMsg;
	reader.Read(syncMsg);
	_objectHandler.SyncNetVar(syncMsg);
}

// ------------------------------------------------------------

void CClientObjectMessageProcessor::ProcessNetFuncCall(INetMessageReader& reader)
{
	CObjectMsg_NetFuncCall callMsg;
	reader.Read(callMsg);
	_objectHandler.ProcessNetFuncCall(callMsg);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
