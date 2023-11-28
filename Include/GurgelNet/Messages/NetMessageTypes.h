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
	ENetMsgCategory_Internal	= 0x00, 
	ENetMsgCategory_Custom		= 0x01, // This should be your start when creating custom message types
};