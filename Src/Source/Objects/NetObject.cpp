#include "GurgelNet/Objects/NetObject.h"
#include "GurgelNet/Serialization/INetSerializer.h"

void CNetObject::SetNetObjectID(NetObjectID id)
{
	_objectID = id;
}

NetObjectID CNetObject::GetNetObjectID() const
{
	return _objectID;
}

bool CNetObject::IsClient() const
{
	return !_isServer;
}

bool CNetObject::IsServer() const
{
	return _isServer;
}

void CNetObject::OnNetworkSpawn(INetObjectInitializer& initializer)
{
	_isServer = initializer.IsServer();
}
