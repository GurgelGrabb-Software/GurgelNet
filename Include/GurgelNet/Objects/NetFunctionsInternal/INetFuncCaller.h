#pragma once
#include "GurgelNet/Core/NetTypes.h"

// ------------------------------------------------------------

class INetMessageWriter;

// ------------------------------------------------------------

class INetFuncCaller
{
public:
	virtual void PrepCall(NetFuncID forID) = 0;
	virtual INetMessageWriter& Writer() = 0;
	virtual void DoCall() = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------