#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "Src/Include/NetLayerMessages/BaseNetLayerMessages.h"

#include "Src/Include/Objects/NetObjectHandle.h"

class CObjectMsg_Spawn : public TObjectMsg<EObjectMsg_Spawn>
{
public:
	NetObjectID spawnedObjectID = NetObjectID_Unset;
	NetTypeID spawnedTypeID = NetTypeID_Unset;
	ClientID ownerID = ClientID_None;
	SNetObjectHandle* objectHandle = nullptr;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectHandle->objectPtr->GetNetObjectID());
		serializer.Write(objectHandle->objectPtr->GetNetTypeID());
		serializer.Write(objectHandle->objectPtr->GetOwnerMask());
		objectHandle->objectPtr->WriteSpawnData(serializer);
		objectHandle->netVariables.WriteNetVarStates(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(spawnedObjectID);
		serializer.Read(spawnedTypeID);
		serializer.Read(ownerID);
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
	ClientID ownerID = ClientID_None;
	SNetObjectHandle* objectHandle = nullptr;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectHandle->objectPtr->GetNetObjectID());
		serializer.Write(objectHandle->objectPtr->GetNetTypeID());
		serializer.Write(ownerID);
		objectHandle->objectPtr->WriteSpawnData(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_reader = &serializer;
		serializer.Read(pendingSpawnID);
		serializer.Read(spawnedTypeID);
		serializer.Read(ownerID);
	}

	void ReadPreSpawnData(SNetObjectHandle& handle)
	{
		handle.objectPtr->ReadSpawnData(*_reader);
	}

private:
	INetMessageReader* _reader = nullptr;
};

class CObjectMsg_SpawnConfirm : public TObjectMsg<EObjectMsg_SpawnConfirm>
{
public:
	NetObjectID pendingID;
	NetObjectID confirmedID;
	ClientID ownerMask;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(pendingID);
		serializer.Write(confirmedID);
		serializer.Write(ownerMask);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(pendingID);
		serializer.Read(confirmedID);
		serializer.Read(ownerMask);
	}
};

class CObjectMsg_Despawn : public TObjectMsg<EObjectMsg_Despawn>
{
public:
	NetObjectID id;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(id);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(id);
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

class CObjectMsg_NetFuncCall : public TObjectMsg<EObjectMsg_NetFuncCall>
{
public:
	NetObjectID objectID;
	NetFuncID functionID;
	INetMessageWriter* customWriter;

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(objectID);
		serializer.Write(functionID);
		serializer.Write(customWriter->RawData(), customWriter->Size());
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		_readerPtr = &serializer;
		serializer.Read(objectID);
		serializer.Read(functionID);
	}

	INetMessageReader& GetReader()
	{
		return *_readerPtr;
	}

private:
	INetMessageReader* _readerPtr;
};