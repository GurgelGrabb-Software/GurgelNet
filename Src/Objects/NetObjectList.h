#pragma once
#include <vector>
#include "GurgelNet/Objects/NetObject.h"

class CNetObjectList
{
public:
	CNetObjectList();

	void AddWithID(CNetObject* object, NetObjectID id);

	NetObjectID Add(CNetObject* object);
	void Remove(NetObjectID id);
	CNetObject* Get(NetObjectID id);

	bool IsIDValid(NetObjectID id) const;

private:
	bool TryGetFreeSlot(NetObjectID& outID);
	void IncreaseIDSlotCount();
	NetObjectID GetCurrentIDSlotCeiling() const;

	std::vector<CNetObject*> _objects;
	std::vector<NetObjectID> _freeIDs;
};