// GurgelNet - 2023
// ------------------------------------------------------------
// INetMessage.h 
// ------------------------------------------------------------
// Interface allowing you to create your own custom messages
// 
// A net message is a net serializable with category and sub-
// IDs
// ------------------------------------------------------------

#pragma once

#include "GurgelNet/Serialization/INetSerializable.h"
#include <cstdint>

// ------------------------------------------------------------

class INetMessage : public INetSerializable
{
public:
	virtual uint8_t CategoryID() const = 0;
	virtual uint8_t SubTypeID() const = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------