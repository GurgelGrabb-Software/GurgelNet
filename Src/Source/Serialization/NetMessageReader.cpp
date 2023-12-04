#include "Src/Include/Serialization/NetMessageReader.h"
#include "Src/Include/Messaging/NetMessage.h"
#include "GurgelNet/Serialization/INetSerializable.h"

#include <cstring>

CNetMessageReader::CNetMessageReader()
	: _dataPtr(nullptr)
	, _dataSize(0u)
	, _readHead(0u)
{
}

CNetMessageReader::CNetMessageReader(SNetMessage&& message) noexcept
	: _dataPtr((uint8_t*)message.pData)
	, _dataSize(message.nBytes)
	, _readHead(0u)
{
	message.pData = nullptr;
	message.nBytes = 0u;
}

CNetMessageReader::CNetMessageReader(void* data, size_t size)
	: _dataPtr((uint8_t*)data)
	, _dataSize(size)
	, _readHead(0u)
{
}

CNetMessageReader::~CNetMessageReader()
{
	if (_dataPtr)
	{
		delete _dataPtr;
	}
}

void CNetMessageReader::Serialize(void* ptr, size_t s)
{
	Read(ptr, s);
}

void CNetMessageReader::Serialize(INetSerializable& serializable)
{
	Read(serializable);
}

void CNetMessageReader::Read(void* ptr, size_t s)
{
	memcpy_s(ptr, s, &_dataPtr[_readHead], s);
	_readHead += s;
}

void CNetMessageReader::Read(INetSerializable& serializable)
{
	serializable.Deserialize(*this);
}