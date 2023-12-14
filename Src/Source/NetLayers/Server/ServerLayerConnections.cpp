#include "Src/Include/NetLayers/Server/ServerLayerConnections.h"

#include "Src/Include/NetLayers/NetLayerContext.h"
#include "Src/Include/Objects/ServerObjectHandler.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include "Src/Core/Logging.h"

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
	NETLOG_SERVER(ENetLogLevel_Confirm, "Opening connection {}, assigned ID {}", hConnection, id);

	ReserveID(id);
	_connectionHandles.push_back(SClientConnection{ .hConnection = hConnection, .clientID = id, .active = false });

	// Run backend setup
	auto backendInterface = _netContext.backend.interfacePtr;
	auto backendPollGroup = _netContext.backend.hPollGroup;

	backendInterface->AcceptConnection(hConnection);
	backendInterface->SetConnectionPollGroup(hConnection, backendPollGroup);
}

// ------------------------------------------------------------

void SServerLayerConnections::CloseConnectionByHandle(unsigned int hConnection, CServerObjectHandler* objectHandler)
{
	// Close the backend
	auto backendInterface = _netContext.backend.interfacePtr;
	backendInterface->CloseConnection(hConnection, 0, "", false);

	// Erase the connection if it exists in our connections
	if (auto connectionPtr = GetConnectionByHandle(hConnection))
	{
		NETLOG_SERVER(ENetLogLevel_Message, "Client (client ID {}) disconnected", connectionPtr->clientID );
		
		if(objectHandler) 
			objectHandler->DespawnAllClientObjects(connectionPtr->clientID);

		ReleaseID(connectionPtr->clientID);
		if (_netContext.analyzerPtr) _netContext.analyzerPtr->UpdateConnectionCount((unsigned int)_connectionHandles.size());
	}
	else
	{
		NETLOG_SERVER(ENetLogLevel_Message, "Connection (ID {}) closed before assigned client ID", hConnection);
	}
}

// ------------------------------------------------------------

void SServerLayerConnections::CloseConnectionByID(ClientID id)
{
	CloseConnectionByHandle(GetConnectionByID(id)->hConnection, nullptr);
}

// ------------------------------------------------------------

void SServerLayerConnections::SetConnectionActive(ClientID id)
{
	NETLOG_SERVER(ENetLogLevel_Confirm, "Client with ID {} has finalized approval and late join. Now active.", id);

	if (auto connectionPtr = GetConnectionByID(id))
	{
		connectionPtr->active = true;
		if (_netContext.analyzerPtr)_netContext.analyzerPtr->UpdateConnectionCount((unsigned int)_connectionHandles.size());
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