#include "Src/Include/Serialization/NetMessageWriter.h"

#include "GurgelNet/Serialization/INetSerializable.h"

CNetMessageWriter::CNetMessageWriter()
	: _byteBuffer()
{
}

void CNetMessageWriter::Serialize(void* ptr, size_t s)
{
	Write(ptr, s);
}

void CNetMessageWriter::Serialize(INetSerializable& serializable)
{
	Write(serializable);
}

void CNetMessageWriter::Write(const void* ptr, size_t s)
{
	const uint8_t* byteArrayPtr = static_cast<const uint8_t*>(ptr);

	for (size_t i = 0; i < s; ++i)
	{
		_byteBuffer.push_back(byteArrayPtr[i]);
	}
}

void CNetMessageWriter::Write(const INetSerializable& serializable)
{
	serializable.Serialize(*this);
}

size_t CNetMessageWriter::Size() const
{
	return _byteBuffer.size();
}

void CNetMessageWriter::Pack(void* target)
{
	memcpy_s(target, _byteBuffer.size(), _byteBuffer.data(), _byteBuffer.size());
	_byteBuffer.clear();
}
