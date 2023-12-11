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
		}
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
		netFuncList->Reset();
	}

	bool Valid() const
	{
		return objectPtr != nullptr;
	}

	CNetObject* objectPtr;
	CNetFuncList* netFuncList;
	CNetVarList netVariables;
};