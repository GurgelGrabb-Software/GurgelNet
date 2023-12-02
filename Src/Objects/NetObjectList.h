#pragma once
#include <vector>
#include "GurgelNet/Objects/NetObject.h"

class CNetworkVariable;
class INetMessageQueue;

struct SNetObjectHandle
{
	CNetObject* objectPtr;
	std::vector<CNetworkVariable*> variables;
};

class CNetObjectList
{
public:
	CNetObjectList();

	void AddWithID(CNetObject* object, NetObjectID id);

	NetObjectID Add(CNetObject* object);
	void Remove(NetObjectID id);
	CNetObject* Get(NetObjectID id);

	bool IsIDValid(NetObjectID id) const;

	void RegisterVariable(NetObjectID id, CNetworkVariable& var);
	CNetworkVariable* GetNetVar(NetObjectID objectID, NetVarID varID);
	void SyncNetworkVariables(INetMessageQueue& messageQueue);



private:
	bool TryGetFreeSlot(NetObjectID& outID);
	void IncreaseIDSlotCount();
	NetObjectID GetCurrentIDSlotCeiling() const;

	std::vector<SNetObjectHandle> _objects;
	std::vector<NetObjectID> _freeIDs;
};