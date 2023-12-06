#pragma once
#include "GurgelNet/Objects/NetFunctionsInternal/INetFuncCaller.h"
#include "Src/Include/Serialization/NetMessageWriter.h"



class CNetFuncList;

// ------------------------------------------------------------

class CNetFuncCaller : public  INetFuncCaller
{
public:
	CNetFuncCaller(CNetFuncList& list, NetObjectID objID);

	void PrepCall(NetFuncID forID) override;
	INetMessageWriter& Writer() override;
	void DoCall() override;

private:
	CNetFuncList& _functionList;
	CNetMessageWriter _writer;
	NetObjectID _objectID;
	NetFuncID _currentID;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------