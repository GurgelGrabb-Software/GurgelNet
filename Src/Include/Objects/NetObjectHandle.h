#pragma once
#include "GurgelNet/Objects/NetObject.h"
#include "Src/Include/Objects/NetVarList.h"
#include "Src/Include/Objects/NetworkFunctions/NetFuncList.h"

class CNetFuncList;

struct SNetObjectHandle
{
	SNetObjectHandle()
		: objectPtr(nullptr)
		, netFuncList(nullptr)
		, netVariables()
	{
	}
	SNetObjectHandle(CNetObject& obj)
		: objectPtr(&obj)
		, netFuncList(nullptr)
		, netVariables()
	{
	}
	SNetObjectHandle(SNetObjectHandle&& o) noexcept
		: objectPtr(o.objectPtr)
		, netFuncList(o.netFuncList)
		, netVariables(std::move(o.netVariables))
	{
		o.objectPtr = nullptr;
		o.netFuncList = nullptr;
	}

	~SNetObjectHandle()
	{
		if (netFuncList)
		{
			delete netFuncList;
			netFuncList = nullptr;
		}
	}
	
	void operator=(SNetObjectHandle&& o) noexcept
	{
		objectPtr = o.objectPtr;
		o.objectPtr = nullptr;
		netVariables = std::move(o.netVariables);
		netFuncList = o.netFuncList;
		o.netFuncList = nullptr;
	}

	void Reset()
	{
		objectPtr = nullptr;
		netVariables.Clear();
		if (netFuncList)
		{
			netFuncList->Reset();
		}
	}

	bool Valid() const
	{
		return objectPtr != nullptr;
	}

	CNetObject* objectPtr;
	CNetFuncList* netFuncList;
	CNetVarList netVariables;
};