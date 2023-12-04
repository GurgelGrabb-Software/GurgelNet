#pragma once
#include "GurgelNet/Serialization/INetSerializer.h"

#include <vector>
#include <cstdint>

class CNetMessageWriter : public INetMessageWriter
{
public:
	CNetMessageWriter();

	void Serialize(void* ptr, size_t s) override;
	void Serialize(INetSerializable& serializable) override;

	void Write(const void* ptr, size_t s) override;
	void Write(const INetSerializable& serializable) override;

	size_t Size() const override;
	void Pack(void* target) override;

private:
	std::vector<uint8_t> _byteBuffer;
};