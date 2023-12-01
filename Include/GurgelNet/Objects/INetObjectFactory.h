// GurgelNet - 2023
// ------------------------------------------------------------
// INetObjectFactory.h 
// ------------------------------------------------------------
// Interface to allow constructing objects remotely via a
// NetTypeID
// 
// You should register this with your network layer to allow
// constructing network objects using your own allocator
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Objects/NetObject.h"

class INetObjectFactory
{
public:
	virtual CNetObject* MakeObject( CNetObject::NetTypeID id ) = 0;
};