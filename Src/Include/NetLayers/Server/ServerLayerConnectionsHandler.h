#pragma once
#include "Src/Include/NetLayers/Server/ServerLayerConnections.h"

class INetMessageReader;
class CNetLayerServer;
class CServerObjectHandler;

class CServerLayerConnectionsHandler
{
public:
	CServerLayerConnectionsHandler(CNetLayerServer& serverLayer, SNetLayerContext& netContext);

	void IncomingConnection(unsigned int hConnection);
	void EstablishedConnection(unsigned int hConnection);
	void ClosedConnection(unsigned int hConnection);

	void RunClientApproval(ClientID clientID, INetMessageReader& approvalDataReader);
	void LateJoinCompleted(ClientID clientID);

	const SServerLayerConnections& GetConnections() const;
private:
	CNetLayerServer& _serverLayer;
	SNetLayerContext& _netContext;
	SServerLayerConnections _connections;
};