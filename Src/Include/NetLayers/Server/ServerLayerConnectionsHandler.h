#pragma once
#include "Src/Include/NetLayers/Server/ServerLayerConnections.h"

class INetMessageReader;

class CServerLayerConnectionsHandler
{
public:
	CServerLayerConnectionsHandler(SNetLayerContext& netContext);

	void IncomingConnection(unsigned int hConnection);
	void EstablishedConnection(unsigned int hConnection);
	void ClosedConnection(unsigned int hConnection);

	void RunClientApproval(ClientID clientID, INetMessageReader& approvalDataReader);
	void LateJoinCompleted(ClientID clientID);

	const SServerLayerConnections& GetConnections() const;
private:
	SNetLayerContext& _netContext;
	SServerLayerConnections _connections;
};