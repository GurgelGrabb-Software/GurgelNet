#pragma once
#include <cstdint>
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/INetMessage.h"
#include "GurgelNet/Serialization/INetSerializer.h"

enum EInternalMsgT : uint8_t
{
	EInternalMsgT_AssignedID,
};

struct SAssignedIDMsg : public INetMessage
{
	uint8_t ID;
	SAssignedIDMsg(uint8_t id) : ID(id) {}

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsgT_AssignedID; }

	void Serialize(INetMessageWriter& serializer) const override { serializer.Write(ID); }
	void Deserialize(INetMessageReader& serializer) override { serializer.Read(ID); }
};