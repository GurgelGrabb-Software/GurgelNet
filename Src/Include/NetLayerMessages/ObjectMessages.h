#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "Src/Include/NetLayerMessages/BaseNetLayerMessages.h"

#include "Src/Include/Objects/NetObjectHandle.h"

class CObjectMsg_Spawn : public TObjectMsg<EObjectMsg_Spawn>
{
public:
	NetObjectID spawnedObjectID = NetObjectID_Unset;
	NetTypeID spawnedTypeID = NetTypeID_Unset;
	SNetObjectHandle* objectHandle = nullptr;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectHandle->objectPtr->GetNetObjectID());
		serializer.Write(objectHandle->objectPtr->GetNetTypeID());
		objectHandle->objectPtr->WriteSpawnData(serializer);
		objectHandle->netVariables.WriteNetVarStates(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(spawnedObjectID);
		serializer.Read(spawnedTypeID);
	}

	void ReadPreSpawnData(SNetObjectHandle& handle)
	{
		handle.objectPtr->ReadSpawnData(*_reader);
	}

	void ReadPostSpawnData(SNetObjectHandle& handle)
	{
		handle.netVariables.ReadNetVarStates(*_reader);
	}

private:
	INetMessageReader* _reader;
};

class CObjectMsg_SpawnRequest : public TObjectMsg<EObjectMsg_SpawnRequest>
{
public:
	NetObjectID pendingSpawnID = NetObjectID_Unset;
	NetTypeID spawnedTypeID = NetTypeID_Unset;
	SNetObjectHandle* objectHandle = nullptr;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectHandle->objectPtr->GetNetObjectID());
		serializer.Write(objectHandle->objectPtr->GetNetTypeID());
		objectHandle->objectPtr->WriteSpawnData(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(pendingSpawnID);
		serializer.Read(spawnedTypeID);
	}

	void ReadPreSpawnData(SNetObjectHandle& handle)
	{
		handle.objectPtr->ReadSpawnData(*_reader);
	}

private:
	INetMessageReader* _reader;
};

class CObjectMsg_SpawnConfirm : public TObjectMsg<EObjectMsg_SpawnConfirm>
{
public:
	NetObjectID pendingID;
	NetObjectID confirmedID;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(pendingID);
		serializer.Write(confirmedID);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(pendingID);
		serializer.Read(confirmedID);
	}
};

class CObjectMsg_NetVarSync : public TObjectMsg<EObjectMsg_NetVarSync>
{
public:
	NetObjectID objectID;
	NetVarID varID;
	CNetworkVariable* varPtr;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectID);
		serializer.Write(varPtr->GetVariableID());
		serializer.Write(*varPtr);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(objectID);
		serializer.Read(varID);
	}

	void DeserializeNetVarData(CNetworkVariable& var)
	{
		var.Deserialize(*_reader);
	}

private:
	INetMessageReader* _reader;
};