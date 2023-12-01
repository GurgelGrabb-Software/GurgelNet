#pragma once
#include <cstdint>
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/INetMessage.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/LayerCallbacks.h"
#include "GurgelNet/Objects/NetObject.h"
#include "Src/Messages/InternalMessageTypes.h"


struct CInternalMsg_Object_ClientRequestSpawn : public INetMessage
{
	CNetObject::NetTypeID ObjectTypeID;
	CNetObject::NetObjectID PendingID;
	CNetObject* object;
	
	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_Object_ClientRequestSpawn; }

	void Serialize(INetMessageWriter& serializer) const override 
	{
		serializer.Write(ObjectTypeID);
		serializer.Write(PendingID);
		object->WriteSpawnData(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override 
	{
		serializer.Read(ObjectTypeID);
		serializer.Read(PendingID);
	}
};

struct CInternalMsg_Object_Spawn : public INetMessage
{
	CNetObject::NetTypeID ObjectTypeID;
	CNetObject::NetObjectID ObjectID;
	CNetObject* object;

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_Object_Spawn; }

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(ObjectTypeID);
		serializer.Write(ObjectID);
		object->DoSpawnWrite(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(ObjectTypeID);
		serializer.Read(ObjectID);
	}
};

struct CInternalMsg_Object_ServerConfirmSpawn : public INetMessage
{
	CNetObject::NetObjectID PendingID;
	CNetObject::NetObjectID ConfirmedID;

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_Object_ServerConfirmSpawn; }

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(PendingID);
		serializer.Write(ConfirmedID);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(PendingID);
		serializer.Read(ConfirmedID);
	}
};