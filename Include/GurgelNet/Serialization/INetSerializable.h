// GurgelNet - 2023
// ------------------------------------------------------------
// NetSerializable.h 
// ------------------------------------------------------------
// Interface for custom serializable types
// 
// Note that trivially constructible types do not need to 
// inherit from this interface in order to allow serialization
// ------------------------------------------------------------

#pragma once

// ------------------------------------------------------------

class INetMessageSerializer;
class INetMessageReader;
class INetMessageWriter;

// ------------------------------------------------------------

class INetSerializable
{
public:
	/// <summary>
	/// Serialize (write) this serializables data to a message
	/// </summary>
	/// <param name="serializer">The write serializer for the message</param>
	virtual void Serialize(INetMessageWriter& serializer) const = 0;

	/// <summary>
	/// Read data from a network message into this serializable
	/// </summary>
	/// <param name="serializer">The read serializer containing the message data</param>
	virtual void Deserialize(INetMessageReader& serializer) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------