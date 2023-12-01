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