#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Objects/NetFunctionsInternal/Locality.h"
#include "GurgelNet/Objects/NetFunctionsInternal/INetFuncCaller.h"
#include "GurgelNet/Serialization/INetSerializer.h"

// ------------------------------------------------------------

class INetFuncCaller;

// ------------------------------------------------------------

class CNetFuncHandle
{
public:
	CNetFuncHandle(ENetFuncLocality loc);

	virtual void RunInvoke(INetMessageReader& r) = 0;

	void Initialize(NetFuncID internalID, bool serverObj, INetFuncCaller* callerPtr);
	ENetFuncLocality GetLocality() const { return _locality; }
protected:
	bool InvokeRemote();
	void DoInvokeRemote();

	template<typename T>
	void Write(const T& w)
	{
		_callerPtr->Writer().Write(w);
	}

private:
	ENetFuncLocality _locality;
	INetFuncCaller* _callerPtr;

	NetFuncID _id;
	
	bool _invokeRemote;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------