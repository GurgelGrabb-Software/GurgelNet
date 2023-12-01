#pragma once
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/INetMessage.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/Objects/NetworkVariable.h"

#include "Src/Messages/InternalMessageTypes.h"

class CInternalMsg_NetVarSync : public INetMessage
{
public:
	NetObjectID ObjectID;
	NetVarID VarID;
	CNetworkVariable* VarPtr;
	
	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_NetVarSync; }

	void Serialize(INetMessageWriter& serializer) const override
	{
		serializer.Write(ObjectID);
		serializer.Write(VarID);
		VarPtr->Serialize(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		serializer.Read(ObjectID);
		serializer.Read(VarID);
	}
};