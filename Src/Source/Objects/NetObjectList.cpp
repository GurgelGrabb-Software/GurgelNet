#include "Src/Include/Objects/NetObjectList.h"

// ------------------------------------------------------------

CNetObjectList::CNetObjectList()
	: _freeIDs()
	, _objects()
{
}

// ------------------------------------------------------------

NetObjectID CNetObjectList::Insert(CNetObject* object)
{
	NetObjectID givenID = GetFreeID();
	_objects[givenID] = SNetObjectHandle(*object);
	object->_objectID = givenID;
	return givenID;
}

// ------------------------------------------------------------

void CNetObjectList::InsertWithID(CNetObject* object, NetObjectID id)
{
	while (_objects.size() <= static_cast<size_t>(id))
	{
		IncreaseSize(id);
	}

	_objects[id] = SNetObjectHandle(*object);
	object->_objectID = id;
}

// ------------------------------------------------------------

void CNetObjectList::Remove(NetObjectID id)
{
	SNetObjectHandle& handle = GetObject(id);
	handle.objectPtr->_objectID = NetObjectID_Unset;
	handle.Reset();
	_freeIDs.push(id);
}

unsigned int CNetObjectList::NumObjects() const
{
	return (unsigned int)(_objects.size() - _freeIDs.size());
}

// ------------------------------------------------------------

SNetObjectHandle& CNetObjectList::GetObject(NetObjectID id)
{
	return _objects[id];
}

// ------------------------------------------------------------

const SNetObjectHandle& CNetObjectList::GetObject(NetObjectID id) const
{
	return _objects[id];
}

// ------------------------------------------------------------

void CNetObjectList::ForEach(std::function<void(SNetObjectHandle&)> forEach)
{
	for (auto& handle : _objects)
	{
		if(handle.objectPtr != nullptr)
			forEach(handle);
	}
}

// ------------------------------------------------------------

void CNetObjectList::IncreaseSize()
{
	IncreaseSize(NetObjectID_Unset);
}

// ------------------------------------------------------------

void CNetObjectList::IncreaseSize(NetObjectID reserveID)
{
	const auto oldSize = _objects.size();
	const auto newSize = oldSize + 32u;
	_objects.resize(newSize);

	for (auto slotIdx = oldSize; slotIdx < newSize; ++slotIdx)
	{
		if (slotIdx != reserveID)
		{
			_freeIDs.push((NetObjectID)slotIdx);
		}
	}
}

// ------------------------------------------------------------

NetObjectID CNetObjectList::GetFreeID()
{
	if (_freeIDs.empty())
	{
		IncreaseSize();
	}

	NetObjectID freeID = _freeIDs.front();
	_freeIDs.pop();
	return freeID;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------