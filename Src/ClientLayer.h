#pragma once
#include "Src/BaseNetLayer.h"
#include "GurgelNet/LayerCallbacks.h"
#include "Src/Objects/NetObjectList.h"

#include <string>

class CClientLayer : public CNetLayerBase
{
public:
	CClientLayer(const std::string& ip, unsigned short port);

	virtual void Start() override;
	virtual void Shutdown() override;

	void RecieveMessages() override;
	void SendQueuedMessages() override;
	
	void Send(const INetMessage& message, bool reliable = false) override;
	void Send(const INetMessage& message, ClientID targetMask, bool reliable = false) override;


	void SpawnNetworkObject(CNetObject& spawn) override;
	void ProcessObjectSpawn(CNetObject& spawned, NetObjectID objID);
	void ConfirmNetworkObjectSpawn(NetObjectID pendingID, NetObjectID confirmedID);

	void Connecting();
	void Connected();
	void Disconnected();
	void ConnectionFinalized();

	static CClientLayer* s_instancePtr;
private:
	void NotifyClientConnection(ClientID id);

	std::string _ip;
	unsigned short _port;

	CNetObjectList _pendingObjectList;
	CNetObjectList _activeObjectList;

	unsigned int _connection;
	
};