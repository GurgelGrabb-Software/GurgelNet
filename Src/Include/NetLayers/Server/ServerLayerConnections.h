#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include <vector>

struct SNetLayerContext;

// ------------------------------------------------------------

struct SClientConnection
{
public:
	unsigned int hConnection	= 0;
	ClientID clientID			= ClientID_None;
	bool active					= false;
};

// ------------------------------------------------------------

struct SServerLayerConnections
{
public:
	SServerLayerConnections(SNetLayerContext& hBackend);

	// ------------------------------------------------------------
	// Connection Processing
	// ------------------------------------------------------------
	bool TryFindFreeID(ClientID& outID);
	void OpenConnection(unsigned int hConnection, ClientID id);
	void CloseConnectionByHandle(unsigned int hConnection);
	void CloseConnectionByID(ClientID id);
	void SetConnectionActive(ClientID id);

	// ------------------------------------------------------------
	// Connection Accessing
	// ------------------------------------------------------------
	SClientConnection* GetConnectionByHandle(unsigned int hConnection);
	SClientConnection* GetConnectionByID(ClientID id);
	
	const std::vector<SClientConnection>& GetConnectionsList() const;
private:
	void ReserveID(ClientID id);
	void ReleaseID(ClientID id);

	SNetLayerContext& _netContext;

	ClientID _freeIDs;
	std::vector<SClientConnection> _connectionHandles;
};