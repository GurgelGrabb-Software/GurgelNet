#pragma once
#include "GurgelNet/Core/NetTypes.h"

#include "GurgelNet/Serialization/INetSerializable.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/Objects/NetObject.h"
#include "Src/Include/Objects/NetObjectHandle.h"

#include <vector>


struct SLateJoinObjectPayload : public INetSerializable
{
	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objHandlePtr->objectPtr->GetNetObjectID());
		serializer.Write(objHandlePtr->objectPtr->GetNetTypeID());
		objHandlePtr->objectPtr->WriteSpawnData(serializer);
		objHandlePtr->netVariables.WriteNetVarStates(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(objectID);
		serializer.Read(objectTypeID);
	}

	SNetObjectHandle* objHandlePtr;
	NetObjectID objectID;
	NetTypeID objectTypeID;
};

class CLateJoinPayload : public INetSerializable
{
public:
	NetObjectID numObjects = 0;

	void AddObject(SNetObjectHandle& handle)
	{
		SLateJoinObjectPayload& added = allObjects.emplace_back();
		added.objHandlePtr = &handle;
	}

	void Serialize(INetMessageWriter& serializer) const override
	{
		NetObjectID num = (NetObjectID)allObjects.size();
		serializer.Write(num);

		for (auto& objPL : allObjects)
		{
			serializer.Write(objPL);
		}
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(numObjects);
	}

	void DeserializeNextObject(SLateJoinObjectPayload& to)
	{
		_reader->Read(to);
	}

	void PreSpawnRead(CNetObject* object)
	{
		object->ReadSpawnData(*_reader);
	}

	void PostSpawnRead(SNetObjectHandle& handle)
	{
		handle.netVariables.ReadNetVarStates(*_reader);
	}

private:
	INetMessageReader* _reader;
	std::vector<SLateJoinObjectPayload> allObjects;

};