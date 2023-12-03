#include "Src/Include/NetLayers/Server/ServerLayerConnections.h"

#include "Src/Include/NetLayers/NetLayerContext.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

// ------------------------------------------------------------

SServerLayerConnections::SServerLayerConnections(SNetLayerContext& context)
	: _netContext(context)
	, _freeIDs(ClientID_AllClients)
	, _connectionHandles()
{
}

// ------------------------------------------------------------
// Connection Processing
// ------------------------------------------------------------

bool SServerLayerConnections::TryFindFreeID(ClientID& outID)
{
	for (ClientID idShift = 0; idShift < 7; ++idShift)
	{
		const ClientID checkID = 1u << idShift;
		if ((_freeIDs & checkID) > 0)
		{
			outID = checkID;
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------

void SServerLayerConnections::OpenConnection(unsigned int hConnection, ClientID id)
{
	ReserveID(id);
	_connectionHandles.push_back(SClientConnection{ .hConnection = hConnection, .clientID = id, .active = false });

	// Run backend setup
	auto backendInterface = _netContext.backend.interfacePtr;
	auto backendPollGroup = _netContext.backend.hPollGroup;

	backendInterface->AcceptConnection(hConnection);
	backendInterface->SetConnectionPollGroup(hConnection, backendPollGroup);
}

// ------------------------------------------------------------

void SServerLayerConnections::CloseConnectionByHandle(unsigned int hConnection)
{
	// Close the backend
	auto backendInterface = _netContext.backend.interfacePtr;
	backendInterface->CloseConnection(hConnection, 0, "", false);

	// Erase the connection if it exists in our connections
	if (auto connectionPtr = GetConnectionByHandle(hConnection))
	{
		ReleaseID(connectionPtr->clientID);
	}
}

// ------------------------------------------------------------

void SServerLayerConnections::CloseConnectionByID(ClientID id)
{
	CloseConnectionByHandle(GetConnectionByID(id)->hConnection);
}

// ------------------------------------------------------------

void SServerLayerConnections::SetConnectionActive(ClientID id)
{
	if (auto connectionPtr = GetConnectionByID(id))
	{
		connectionPtr->active = true;
	}
}

// ------------------------------------------------------------
// Connection Accessing
// ------------------------------------------------------------

SClientConnection* SServerLayerConnections::GetConnectionByHandle(unsigned int hConnection)
{
	auto findF = [hConnection](const SClientConnection& c) { return c.hConnection == hConnection; };
	auto it = std::find_if(_connectionHandles.begin(), _connectionHandles.end(), findF);
	return it != _connectionHandles.end() ? &(*it) : nullptr;
}

// ------------------------------------------------------------

SClientConnection* SServerLayerConnections::GetConnectionByID(ClientID id)
{
	auto findF = [id](const SClientConnection& c) { return c.clientID == id; };
	auto it = std::find_if(_connectionHandles.begin(), _connectionHandles.end(), findF);
	return it != _connectionHandles.end() ? &(*it) : nullptr;
}

// ------------------------------------------------------------

const std::vector<SClientConnection>& SServerLayerConnections::GetConnectionsList() const
{
	return _connectionHandles;
}

// ------------------------------------------------------------

void SServerLayerConnections::ReserveID(ClientID id)
{
	_freeIDs = ClientMask_Remove(_freeIDs, id);
}

// ------------------------------------------------------------

void SServerLayerConnections::ReleaseID(ClientID id)
{
	auto connectionIT = std::find_if(_connectionHandles.begin(), _connectionHandles.end(), [id](const auto& h) { return h.clientID == id; });
	_connectionHandles.erase(connectionIT);

	_freeIDs = ClientMask_Add(_freeIDs, id);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------