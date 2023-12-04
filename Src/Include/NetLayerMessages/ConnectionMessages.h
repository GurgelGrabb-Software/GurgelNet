#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "Src/Include/NetLayerMessages/BaseNetLayerMessages.h"
#include "GurgelNet/LayerCallbacks.h"

#include "Src/Include/NetLayerMessages/LateJoinPayload.h"

class CConnectMsg_AssignID : public TConnectMsg< EConnectMsg_AssignClientID >
{
public:
	ClientID id = ClientID_None;

	void Serialize(INetMessageWriter& serializer) const override { serializer.Write(id); }
	void Deserialize(INetMessageReader& serializer) override { serializer.Read(id); }
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
	CLateJoinPayload lateJoinPayload;
	
	void Serialize(INetMessageWriter& serializer) const override 
	{ 
		if (lateJoinWrite)
		{
			lateJoinWrite(serializer); 
		}

		serializer.Write(lateJoinPayload);
	}

	void Deserialize(INetMessageReader& serializer) override 
	{ 
		if (lateJoinRead)
		{
			lateJoinRead(serializer);
		}

		serializer.Read(lateJoinPayload);
	}
};

class CConnectMsg_LateJoinConfirm : public TConnectMsg<EConnectMsg_LateJoinComplete>
{
public:
};

class CConnectMsg_ConnectionActive : public TConnectMsg<EConnectMsg_ConnectionActive>
{
};