#include "GurgelNet/Objects/NetFunctionsInternal/NetFunctionHandle.h"
#include "GurgelNet/Objects/NetFunctionsInternal/INetFuncCaller.h"

// ------------------------------------------------------------

CNetFuncHandle::CNetFuncHandle(ENetFuncLocality loc)
	: _locality(loc)
	, _callerPtr(nullptr)
	, _id(NetFuncID_Unset)
	, _invokeRemote(false)
{
}

// ------------------------------------------------------------

void CNetFuncHandle::Initialize(NetFuncID internalID, bool serverObj, INetFuncCaller* callerPtr)
{
	const auto loc = GetLocality();
	switch (loc)
	{
	case ENetFuncLocality_Client:
		_invokeRemote = serverObj;
		break;
	case ENetFuncLocality_Server:
		_invokeRemote = !serverObj;
		break;
	}

	_id = internalID;
	_callerPtr = callerPtr;
}

// ------------------------------------------------------------

bool CNetFuncHandle::InvokeRemote()
{
	if (_invokeRemote)
		_callerPtr->PrepCall(_id);
	return _invokeRemote;
}

// ------------------------------------------------------------

void CNetFuncHandle::DoInvokeRemote()
{
	_callerPtr->DoCall();
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------