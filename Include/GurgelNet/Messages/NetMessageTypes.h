// GurgelNet - 2023
// ------------------------------------------------------------
// NetMessageTypes.h 
// ------------------------------------------------------------
// Categories for messages
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/NetLib.h"
#include <cstdint>

enum ENetMsgCategory : uint8_t
{
	ENetMsgCategory_Internal	= 0x00, // TODO: Remove this :)

	ENetMsgCategory_Connection,	// Internal category for handling connections (handshakes, approval etc.)
	ENetMsgCategory_Object,		// Internal category for handling objects (spawns, despawns etc.)

	ENetMsgCategory_Custom,			// This should be your start when creating custom message types
};