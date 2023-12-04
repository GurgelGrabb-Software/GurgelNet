#pragma once
#include "Src/Include/Objects/NetObjectHandle.h"

#include <functional>
#include <vector>
#include <queue>

// ------------------------------------------------------------

class CNetObjectList
{
public:
	CNetObjectList();

	NetObjectID Insert(CNetObject* object);
	void InsertWithID(CNetObject* object, NetObjectID id);

	void Remove(NetObjectID id);

	SNetObjectHandle& GetObject(NetObjectID id);
	const SNetObjectHandle& GetObject(NetObjectID id) const;

	void ForEach(std::function<void(SNetObjectHandle&)> forEach);
private:
	void IncreaseSize();
	void IncreaseSize(NetObjectID reserveID);
	NetObjectID GetFreeID();

	std::queue<NetObjectID> _freeIDs;
	std::vector<SNetObjectHandle> _objects;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------