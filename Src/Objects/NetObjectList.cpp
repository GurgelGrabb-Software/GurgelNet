#include "NetObjectList.h"
#include "GurgelNet/Messages/INetMessageQueue.h"
#include "GurgelNet/Objects/NetworkVariable.h"

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

	_objects[id].objectPtr = object;
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
	_objects[id].objectPtr = nullptr;
	_freeIDs.push_back(id);
}

CNetObject* CNetObjectList::Get(NetObjectID id)
{
	return _objects[id].objectPtr;
}

bool CNetObjectList::IsIDValid(NetObjectID id) const
{
	return _objects[id].objectPtr != nullptr;
}

void CNetObjectList::RegisterVariable(NetObjectID id, CNetworkVariable& var)
{
	_objects[id].variables.push_back(&var);
}

CNetworkVariable* CNetObjectList::GetNetVar(NetObjectID objectID, NetVarID varID)
{
	return _objects[objectID].variables[varID];
}

void CNetObjectList::SyncNetworkVariables(INetMessageQueue& messageQueue)
{
	for (auto& objectHandle : _objects)
	{
		for (auto& variable : objectHandle.variables)
		{
			if (variable->ShouldSync())
			{
				
			}
		}
	}
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
		_objects[idSlot].objectPtr = nullptr;
		_freeIDs.push_back(idSlot);
	}
}

NetObjectID CNetObjectList::GetCurrentIDSlotCeiling() const
{
	return static_cast<NetObjectID>(_objects.size());
}
