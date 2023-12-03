#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "Src/Include/NetLayerMessages/BaseNetLayerMessages.h"
#include "GurgelNet/LayerCallbacks.h"

class CConnectMsg_AssignID : public TConnectMsg< EConnectMsg_AssignClientID >
{
public:
	ClientID id = ClientID_None;

	void Serialize(INetMessageWriter& serializer) const override;
	void Deserialize(INetMessageReader& serializer) override;
};

class CConnectMsg_ConfirmID : public TConnectMsg< EConnectMsg_ConfirmClientID >
{
public:
	FNetWrite approvalRequestWrite;
	
	void Serialize(INetMessageWriter& serializer) const override { if (approvalRequestWrite) { approvalRequestWrite(serializer); } }
	void Deserialize(INetMessageReader& serializer) override {}
};

class CConnectMsg_LateJoin : public TConnectMsg< EConnectMsg_LateJoinSync >
{
public:
	FNetWrite lateJoinWrite = nullptr;
	FNetRead lateJoinRead = nullptr;
	
	void Serialize(INetMessageWriter& serializer) const override { lateJoinWrite(serializer); }
	void Deserialize(INetMessageReader& serializer) override { lateJoinRead(serializer); }
};

class CConnectMsg_LatJoinConfirm : public TConnectMsg<EConnectMsg_LateJoinComplete>
{
public:
};

class CConnectMsg_ConnectionActive : public TConnectMsg<EConnectMsg_ConnectionActive>
{
};