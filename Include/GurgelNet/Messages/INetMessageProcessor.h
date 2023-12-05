// GurgelNet - 2023
// ------------------------------------------------------------
// INetMessageProcessor.h 
// ------------------------------------------------------------
// Interface for net message processor
// 
// You can create your own processors using this interface
// Registering these with a net layer allows you to have
// custom processing of custom message categories
// ------------------------------------------------------------

#pragma once

#include <cstdint>

// ------------------------------------------------------------

struct SNetMessageHeader;
class INetMessageReader;
class INetLayer;

// ------------------------------------------------------------

class INetMessageProcessor
{
public:
	virtual ~INetMessageProcessor() = default;

	/// <summary>
	/// Get the message category that this processor is for
	/// </summary>
	/// <returns>Category between 1-255</returns>
	virtual uint8_t ProcessedMessageCategory() const = 0;

	/// <summary>
	/// Process the message (provided as message reader)
	/// </summary>
	/// <param name="header">Header of the message</param>
	/// <param name="reader">Reader initialised with the message (header already read)</param>
	/// <param name="messageQueue">Message queue provided in case a response is to be sent</param>
	virtual void Process(const SNetMessageHeader& header, INetMessageReader& reader, INetLayer& netLayer) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------
