// GurgelNet - 2023
// ------------------------------------------------------------
// NetObjectOwnership.h
// ------------------------------------------------------------
// Enum type to define ownership of network objects
// 
// Ownership determines lifetime and write allowance for the
// give object
// ------------------------------------------------------------

#pragma once

#include <cstdint>
#include "GurgelNet/Core/NetTypes.h"

// ------------------------------------------------------------

enum class ENetObjectOwner : uint8_t
{
	Local,		// Spawning client has exclusive write permission to network variables, Object will despawn with client
	Server,		// Server has exclusive write permission to network variables, Object will remain alive until server shutdown or manual despawn
	Clients,	// Anyone but server may write to network variables, Object will remain alive until server shutdown or manual despawn
	Shared,		// Anyone may write to network variables, Object will remain alive until server shutdown or manual despawn
};

// ------------------------------------------------------------

static ClientID NetObjectOwnerMask(ENetObjectOwner ownerEnum, ClientID localID)
{
	switch (ownerEnum)
	{
	case ENetObjectOwner::Local: return localID;
	case ENetObjectOwner::Server: return ClientID_Server;
	case ENetObjectOwner::Clients: return ClientID_AllClients;
	case ENetObjectOwner::Shared: return ClientID_All;
	}
	return ClientID_None;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------