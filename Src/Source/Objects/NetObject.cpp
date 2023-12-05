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

ClientID CNetObject::GetOwnerMask() const
{
	return _objectOwnerMask;
}

bool CNetObject::IsOwner(ClientID id) const
{
	return ClientMask_Contains(_objectOwnerMask, id);
}

bool CNetObject::IsClient() const
{
	return _isClient;
}

bool CNetObject::IsServer() const
{
	return _isServer;
}

void CNetObject::OnNetworkSpawn(INetObjectInitializer& initializer)
{
	_isServer = initializer.IsServer();
	_isClient = !_isServer;
	_objectOwnerMask = initializer.GetOwnerMask();
}
