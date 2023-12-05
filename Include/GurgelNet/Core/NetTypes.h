// GurgelNet - 2023
// ------------------------------------------------------------
// NetTypes.h
// 
// Typedefinitions for networking types and common values
// ------------------------------------------------------------

#pragma once

#include <cstdint>

// ------------------------------------------------------------
// NetTypeID
// ID used to identify types to spawn via network.
// These should be processable by registered INetObjectFactory
// ------------------------------------------------------------
using NetTypeID = uint32_t;
constexpr static NetTypeID NetTypeID_Unset = 0xFFFFFFFF;

// ------------------------------------------------------------
// NetObjectID
// ID for net objects
// ------------------------------------------------------------
using NetObjectID = uint16_t;
constexpr static NetObjectID NetObjectID_Unset = 0xFFFF;

// ------------------------------------------------------------
// ClientID
// Used to identify clients
// To allow identifying the server, it also has a ClientID
// ------------------------------------------------------------
using ClientID = uint8_t;

// ClientID_None means no clients, not even the server. This is an error value for all intents and purposes.
constexpr static ClientID ClientID_None			= 0;

// ClientID_Server is the server, always
constexpr static ClientID ClientID_Server		= 0x01;

// ClientID_All means everyone on the network, including the server
constexpr static ClientID ClientID_All			= 0xFF;

// ClientID_AllClients means everyone on the network except for the server
constexpr static ClientID ClientID_AllClients	= ClientID_All ^ ClientID_Server;

// ClientID_AllExcept is ClientID_All except the IDs contained in the mask m ( ClientID_AllClients can be thought of as ClientID_AllExcept(ClientID_Server) )
constexpr static ClientID ClientID_AllExcept(ClientID m) { return ClientID_All ^ m; }

// Remove the ID r from the mask m
constexpr static ClientID ClientMask_Remove(ClientID m, ClientID r) { return m & ~(r); }

// Add the ID a to the mask m
constexpr static ClientID ClientMask_Add(ClientID m, ClientID a) { return m | a; }

// Check if the mask m contains the ID c
constexpr static bool ClientMask_Contains(ClientID m, ClientID c) { return (m & c) > 0; }

// ------------------------------------------------------------
// HNetLayer
// Handle to a network layer
// ------------------------------------------------------------
using HNetLayer = size_t;
constexpr static HNetLayer HNetLayer_Invalid	= 0;

// Do you REALLY need more than 255 network variables for a single object?
using NetVarID = uint8_t;
constexpr static NetVarID NetVarID_Unset		= 0xFF;