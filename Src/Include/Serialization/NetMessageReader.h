#pragma once
#include "GurgelNet/Serialization/INetSerializer.h"

struct SNetMessage;

class CNetMessageReader : public INetMessageReader
{
public:
	CNetMessageReader();
	CNetMessageReader(SNetMessage&& message) noexcept;
	CNetMessageReader(void* data, size_t size);
	~CNetMessageReader();

	void Serialize(void* ptr, size_t s) override;
	void Serialize(INetSerializable& serializable) override;

	void Read(void* ptr, size_t s) override;
	void Read(INetSerializable& serializable) override;

private:
	const uint8_t* _dataPtr;
	size_t _dataSize;
	size_t _readHead;
};