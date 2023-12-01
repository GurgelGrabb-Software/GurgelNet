#include "NetObjectList.h"

CNetObjectList::CNetObjectList()
{
	IncreaseIDSlotCount();
}

void CNetObjectList::AddWithID(CNetObject* object, NetObjectID id)
{
	while (id >= GetCurrentIDSlotCeiling())
	{
		IncreaseIDSlotCount();
	}

	_objects[id] = object;
}

NetObjectID CNetObjectList::Add(CNetObject* object)
{
	NetObjectID assignedID;
	if (TryGetFreeSlot(assignedID))
	{
		AddWithID(object, assignedID);
	}
	else
	{
		IncreaseIDSlotCount();
		TryGetFreeSlot(assignedID); // Guaranteed success now since we've added empty slots
		AddWithID(object, assignedID);
	}
	
	return assignedID;
}

void CNetObjectList::Remove(NetObjectID id)
{
	_objects[id] = nullptr;
	_freeIDs.push_back(id);
}

CNetObject* CNetObjectList::Get(NetObjectID id)
{
	return _objects[id];
}

bool CNetObjectList::IsIDValid(NetObjectID id) const
{
	return _objects[id] != nullptr;
}

bool CNetObjectList::TryGetFreeSlot(NetObjectID& outID)
{
	if (_freeIDs.size() == 0) return false;

	outID = _freeIDs.back();
	_freeIDs.pop_back();
	return true;
}

void CNetObjectList::IncreaseIDSlotCount()
{
	const NetObjectID oldCeil = static_cast<NetObjectID>(_objects.size());
	const NetObjectID newCeil = oldCeil + 32;

	_objects.resize(newCeil);
	for (NetObjectID idSlot = oldCeil; idSlot < newCeil; ++idSlot)
	{
		_objects[idSlot] = nullptr;
		_freeIDs.push_back(idSlot);
	}
}

NetObjectID CNetObjectList::GetCurrentIDSlotCeiling() const
{
	return static_cast<NetObjectID>(_objects.size());
}
