#include "GurgelNet/Objects/NetObject.h"
#include "GurgelNet/Serialization/INetSerializer.h"

void CNetObject::MarkAsServer()
{
	_isServer = true;
}

void CNetObject::MarkAsClient()
{
	_isClient = true;
}

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
	return _isClient;
}

bool CNetObject::IsServer() const
{
	return _isServer;
}
