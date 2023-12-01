#pragma once
#include <cstdint>
#include "GurgelNet/Messages/NetMessageTypes.h"
#include "GurgelNet/Messages/INetMessage.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/LayerCallbacks.h"

enum EInternalMsgT : uint8_t
{
	EInternalMsg_ServerToClient_ID,					// The server telling a client its uint8_t clientID		(Target = Connecting Client)
	EInternalMsg_ClientToServer_ID,					// The client confirming recieving its uint8_t clientID (Target = Server)

	EInternalMsg_ServerToClient_LateJoinSync,		// The server sending late join data to the client
	EInternalMsg_ClientToServer_LateJoinConfirm,	// The client confirming late join

	EInternalMsg_ServerToClient_NotifyFinalized,	// The server letting the client know that its now finalized and ready to go

	EInternalMsg_Object_ClientRequestSpawn,			// Client requesting spawn of an object (and will expect a confirmation on the network ID)
	EInternalMsg_Object_ServerConfirmSpawn,			// Server confirming the spawn of an object that this client has previously requested
	EInternalMsg_Object_Spawn,						// Server notifying clients about the spawn of an object

	EInternalMsg_NetVarSync,						// Sync of a network variable value
};

struct SHeaderOnlyMsg : public INetMessage
{
	uint8_t _subType;
	SHeaderOnlyMsg() : _subType(0) {}
	SHeaderOnlyMsg(uint8_t subType) : _subType(subType) {}

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return _subType; }

	void Serialize(INetMessageWriter& serializer) const override {}
	void Deserialize(INetMessageReader& serializer) override {}
};

struct SAssignedIDMsg : public INetMessage
{
	uint8_t ID;
	SAssignedIDMsg(uint8_t id) : ID(id) {}

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_ServerToClient_ID; }

	void Serialize(INetMessageWriter& serializer) const override { serializer.Write(ID); }
	void Deserialize(INetMessageReader& serializer) override { serializer.Read(ID); }
};

struct CInternalMsg_Client_ConfirmIDAndRequestApproval : public INetMessage
{
	FNetWrite F_clientWriteApproveRequest;
	FApproveClient F_serverApproveClient;
	bool clientApprovalState;

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_ClientToServer_ID; }

	void Serialize(INetMessageWriter& serializer) const override 
	{
		if (F_clientWriteApproveRequest)
		{
			F_clientWriteApproveRequest(serializer);
		}
	}

	void Deserialize(INetMessageReader& serializer) override 
	{
		clientApprovalState = true;
		if (F_serverApproveClient)
		{
			clientApprovalState = F_serverApproveClient(serializer);
		}
	}
};

struct CInternalMsg_Server_LateJoinSync : public INetMessage
{
	FNetWrite serverWrite = nullptr;
	FNetRead clientRead = nullptr;

	uint8_t CategoryID() const override { return ENetMsgCategory_Internal; }
	uint8_t SubTypeID() const override { return EInternalMsg_ServerToClient_LateJoinSync; }

	void Serialize(INetMessageWriter& serializer) const override
	{
		serverWrite(serializer);
	}

	void Deserialize(INetMessageReader& serializer) override
	{
		clientRead(serializer);
	}
};

