#pragma once
#include "Src/BaseNetLayer.h"
#include <vector>

struct SClientHandle
{
	unsigned int connectionID;
	uint8_t clientID;
};

class CServerLayer : public CNetLayerBase
{
public:
	CServerLayer(unsigned short port);

	void Start() override;
	void Shutdown() override;

	void Recieve() override;
	void Send() override;

	void Connecting(unsigned int connectionID);
	void Connected(unsigned int connectionID);
	void Disconnected(unsigned int connectionID);

	static CServerLayer* s_instancePtr;
private:
	void SendMessage(const SNetMessage& message, int sendFlag);

	void AcceptClientConnection(uint8_t id, unsigned int connectionID);
	void CloseClientConnection(unsigned int connectionID);

	uint8_t _freeClientIDs = 0xFF;

	uint8_t FindFreeClientID() const;
	void FreeClientID(uint8_t id);
	void ReserveClientID(uint8_t id, unsigned int connectionID);
	
	std::vector<SClientHandle> _clientHandles;

	unsigned short _port;

	unsigned int _connection;
	unsigned int _pollGroup;

};