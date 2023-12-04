#pragma once
#include "GurgelNet/Objects/NetObject.h"
#include "Src/Include/Objects/NetVarList.h"

struct SNetObjectHandle
{
	SNetObjectHandle()
		: objectPtr(nullptr)
		, netVariables()
	{
	}
	SNetObjectHandle(CNetObject& obj)
		: objectPtr(&obj)
		, netVariables()
	{
	}
	SNetObjectHandle(SNetObjectHandle&& o) noexcept
		: objectPtr(o.objectPtr)
		, netVariables(std::move(o.netVariables))
	{
		o.objectPtr = nullptr;
	}
	
	void operator=(SNetObjectHandle&& o) noexcept
	{
		objectPtr = o.objectPtr;
		netVariables = std::move(o.netVariables);
		o.objectPtr = nullptr;
	}

	void Reset()
	{
		objectPtr = nullptr;
		netVariables.Clear();
	}

	CNetObject* objectPtr;
	CNetVarList netVariables;
};