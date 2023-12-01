#pragma once
#include "Src/BaseNetLayer.h"
#include "GurgelNet/LayerCallbacks.h"

#include <string>

class CClientLayer : public CNetLayerBase
{
public:
	CClientLayer(const std::string& ip, unsigned short port);

	virtual void Start() override;
	virtual void Shutdown() override;

	void Recieve() override;
	void Send() override;

	void Connecting();
	void Connected();
	void Disconnected();
	void ConnectionFinalized();

	static CClientLayer* s_instancePtr;
private:
	void NotifyClientConnection(ClientID id);

	std::string _ip;
	unsigned short _port;

	unsigned int _connection;
	
};