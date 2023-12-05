// GurgelNet - 2023
// ------------------------------------------------------------
// NetMessageHeader.h 
// ------------------------------------------------------------
// Structure describing data layout and content of the header
// attached to all network messages
// ------------------------------------------------------------

#pragma once

#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetTypes.h"

#include <cstdint>

// ------------------------------------------------------------

struct NETWORK_API SNetMessageHeader
{
	// ID of the original sender. 
	// Messages between clients are routed through the server. In this instance, the senderID will remain that of the sending client
	ClientID senderID;		

	// ID or mask of IDs for target receivers
	ClientID targetID;

	// The "primary type" of the message
	// This ID determines the processor used to process this message
	// Use ENetMsgType_UserMsgStart as the first ID of your own message types as some are reserved for internal communication
	uint8_t categoryID;

	// User data/"secondary type"
	// This can be used by the processor to make additional decisions on how to process the message
	uint8_t subTypeID;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------