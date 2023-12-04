#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetTypes.h"

#include "GurgelNet/Objects/INetObjectInitializer.h"

class INetMessageWriter;
class INetMessageReader;
class CNetworkVariable;

class NETWORK_API CNetObject
{
	friend class CNetObjectList;
public:
	bool IsClient() const;
	bool IsServer() const;

	// Should return the NetTypeID usable by the registered INetObjectFactory
	virtual NetTypeID GetNetTypeID() const = 0;

	// Callback that will be called when the object is spawned
	// Note that for objects spawned on a client, this will be invoked only when the server has confirmed the spawn
	virtual void OnNetworkSpawn(INetObjectInitializer& initializer);

	// Callback that will be called when this network object is either manually despawned or the net library is shutting down
	virtual void OnNetworkDespawn() = 0;

	// Write any additional data belonging to your specific object
	virtual void WriteSpawnData(INetMessageWriter& serializer) = 0;

	// Read any additional data that would have been written by the WriteSpawnData function for this object type
	virtual void ReadSpawnData(INetMessageReader& serializer) = 0;

public:
	virtual ~CNetObject() = default;
	NetObjectID GetNetObjectID() const;

private:
	void SetNetObjectID(NetObjectID id);

	NetObjectID _objectID = NetObjectID_Unset;
	bool _isServer = false;
};