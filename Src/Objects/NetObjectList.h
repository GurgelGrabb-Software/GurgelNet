#pragma once
#include <vector>
#include "GurgelNet/Objects/NetObject.h"

class CNetObjectList
{
public:
	CNetObjectList();

	void AddWithID(CNetObject* object, CNetObject::NetObjectID id);

	CNetObject::NetObjectID Add(CNetObject* object);
	void Remove(CNetObject::NetObjectID id);
	CNetObject* Get(CNetObject::NetObjectID id);

	bool IsIDValid(CNetObject::NetObjectID id) const;

private:
	bool TryGetFreeSlot(CNetObject::NetObjectID& outID);
	void IncreaseIDSlotCount();
	CNetObject::NetObjectID GetCurrentIDSlotCeiling() const;

	std::vector< CNetObject* > _objects;
	std::vector<CNetObject::NetObjectID> _freeIDs;
};