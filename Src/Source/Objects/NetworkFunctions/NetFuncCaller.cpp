#include "Src/Include/Objects/NetworkFunctions/NetFuncCaller.h"
#include "Src/Include/Objects/NetworkFunctions/NetFuncList.h"

#include "GurgelNet/Messages/INetMessageQueue.h"

#include "Src/Include/NetLayerMessages/ObjectMessages.h"

// ------------------------------------------------------------

CNetFuncCaller::CNetFuncCaller(CNetFuncList& list, NetObjectID objID)
	: _functionList(list)
	, _writer()
	, _objectID(objID)
{
}

// ------------------------------------------------------------

void CNetFuncCaller::PrepCall(NetFuncID forID)
{
	_writer.Reset();
	_currentID = forID;
}

// ------------------------------------------------------------

INetMessageWriter& CNetFuncCaller::Writer()
{
	return _writer;
}

// ------------------------------------------------------------

void CNetFuncCaller::DoCall()
{
	CObjectMsg_NetFuncCall funcCall;
	funcCall.objectID = _objectID;
	funcCall.functionID = _currentID;
	funcCall.customWriter = &_writer;

	const ClientID target = _functionList.GetLocalID() == ClientID_Server ? ClientID_AllClients : ClientID_Server;
	_functionList.GetQueue().Send(funcCall, target, true );
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------