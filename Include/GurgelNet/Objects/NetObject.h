#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetTypes.h"

class INetMessageWriter;
class INetMessageReader;

class NETWORK_API CNetObject
{
public:

	// Should return the NetTypeID usable by the registered INetObjectFactory
	virtual NetTypeID GetNetTypeID() const = 0;

	// Callback that will be called when the object is spawned
	// Note that for objects spawned on a client, this will be invoked only when the server has confirmed the spawn
	virtual void OnNetworkSpawn() = 0;

	// Callback that will be called when this network object is either manually despawned or the net library is shutting down
	virtual void OnNetworkDespawn() = 0;

	// Write any additional data belonging to your specific object
	virtual void WriteSpawnData(INetMessageWriter& serializer) = 0;

	// Read any additional data that would have been written by the WriteSpawnData function for this object type
	virtual void ReadSpawnData(INetMessageReader& serializer) = 0;

public:
	virtual ~CNetObject() = default;

	void SetNetObjectID(NetObjectID id);
	NetObjectID GetNetObjectID() const;

private:
	NetObjectID _objectID = NetObjectID_Unset;
};