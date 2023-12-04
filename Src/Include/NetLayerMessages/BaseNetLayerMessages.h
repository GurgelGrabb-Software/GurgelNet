#pragma once
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/INetMessage.h"

#include "Src/Include/NetLayerMessages/NetLayerMessageTypes.h"

template <ENetMsgCategory TCategory, uint8_t TSubType>
class TNetLayerMsg_Base : public INetMessage
{
public:
	virtual ~TNetLayerMsg_Base() = default;

	uint8_t CategoryID() const override { return TCategory; }
	uint8_t SubTypeID() const override { return TSubType; }

	virtual void Serialize(INetMessageWriter& serializer) const override {}
	virtual void Deserialize(INetMessageReader& serializer) override {}
};

template<EConnectMsg TConnectMsgType>
using TConnectMsg = TNetLayerMsg_Base<ENetMsgCategory_Connection, TConnectMsgType>;

template<EObjectMsg TObjMsgType>
using TObjectMsg = TNetLayerMsg_Base<ENetMsgCategory_Object, TObjMsgType>;
