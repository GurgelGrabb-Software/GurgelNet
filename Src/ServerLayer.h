#pragma once
#include "Src/BaseNetLayer.h"
#include "Src/Objects/NetObjectList.h"
#include <vector>

struct SClientHandle
{
	unsigned int connectionID;
	uint8_t clientID;
	bool finalized;
};

class CServerLayer : public CNetLayerBase
{
public:
	CServerLayer(unsigned short port);

	void Start() override;
	void Shutdown() override;

	void Recieve() override;
	void Send() override;

	virtual void Send(const INetMessage& message, bool reliable = false) override;
	virtual void Send(const INetMessage& message, ClientID targetMask, bool reliable = false) override;

	void SpawnNetworkObject(CNetObject& spawn) override;
	void ProcessSpawnRequest(CNetObject& requestedSpawn, ClientID requestClient, NetObjectID pendingID);

	void Connecting(unsigned int connectionID);
	void Connected(unsigned int connectionID);
	void Disconnected(unsigned int connectionID);

	void ApproveClientConnection(uint8_t clientID, bool approved);
	void FinalizeApprovedClientConnection(uint8_t clientID);

	static CServerLayer* s_instancePtr;
private:
	void SendObjectSpawnMessage(CNetObject& forObject, ClientID targetsMask);

	void SendMessage(const SNetMessage& message, int sendFlag);

	void AcceptClientConnection(uint8_t id, unsigned int connectionID);
	void CloseClientConnection(unsigned int connectionID);

	ClientID _freeClientIDs = 0xFF;

	ClientID FindFreeClientID() const;
	void FreeClientID(ClientID id);
	void ReserveClientID(ClientID id, unsigned int connectionID);
	
	std::vector<SClientHandle> _clientHandles;

	CNetObjectList _netObjectList;

	unsigned short _port;

	unsigned int _connection;
	unsigned int _pollGroup;

};