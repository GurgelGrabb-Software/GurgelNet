#include "NetObjectList.h"

CNetObjectList::CNetObjectList()
{
	IncreaseIDSlotCount();
}

void CNetObjectList::AddWithID(CNetObject* object, CNetObject::NetObjectID id)
{
	while (id >= GetCurrentIDSlotCeiling())
	{
		IncreaseIDSlotCount();
	}

	_objects[id] = object;
}

CNetObject::NetObjectID CNetObjectList::Add(CNetObject* object)
{
	CNetObject::NetObjectID assignedID;
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

void CNetObjectList::Remove(CNetObject::NetObjectID id)
{
	_objects[id] = nullptr;
	_freeIDs.push_back(id);
}

CNetObject* CNetObjectList::Get(CNetObject::NetObjectID id)
{
	return _objects[id];
}

bool CNetObjectList::IsIDValid(CNetObject::NetObjectID id) const
{
	return _objects[id] != nullptr;
}

bool CNetObjectList::TryGetFreeSlot(CNetObject::NetObjectID& outID)
{
	if (_freeIDs.size() == 0) return false;

	outID = _freeIDs.back();
	_freeIDs.pop_back();
	return true;
}

void CNetObjectList::IncreaseIDSlotCount()
{
	const CNetObject::NetObjectID oldCeil = static_cast<CNetObject::NetObjectID>(_objects.size());
	const CNetObject::NetObjectID newCeil = oldCeil + 32;

	_objects.resize(newCeil);
	for (CNetObject::NetObjectID idSlot = oldCeil; idSlot < newCeil; ++idSlot)
	{
		_objects[idSlot] = nullptr;
		_freeIDs.push_back(idSlot);
	}
}

CNetObject::NetObjectID CNetObjectList::GetCurrentIDSlotCeiling() const
{
	return _objects.size();
}
