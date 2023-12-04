#include "GurgelNet/Serializables/NetString.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include <cstring>

CNetString::CNetString()
	: _length(0u)
	, _characterArray(nullptr)
{
}

CNetString::CNetString(CNetString&& netStr) noexcept
	: _length(netStr._length)
	, _characterArray(netStr._characterArray)

{
	netStr._length = 0u;
	netStr._characterArray = nullptr;
}

CNetString::CNetString(const CNetString& netStr)
	: _length(netStr._length)
	, _characterArray(new char[netStr._length])
{
	memcpy_s(_characterArray, _length, netStr._characterArray, netStr._length);
}

CNetString::CNetString(const char* cStr, unsigned short len)
	: _length(len)
	, _characterArray(new char[len])
{
	memcpy_s(_characterArray, _length, cStr, len);
}

CNetString::CNetString(const std::string& stdStr)
	: _length(static_cast<unsigned short>(stdStr.length() + 1))
	, _characterArray(new char[stdStr.length() + 1])
{
	memcpy_s(_characterArray, _length, stdStr.c_str(), stdStr.length());
	_characterArray[_length - 1] = '\0';
}

CNetString::~CNetString()
{
	if (_characterArray)
	{
		delete[] _characterArray;
		_characterArray = nullptr;
	}
}

void CNetString::operator=(const std::string& stdStr)
{
	if (_characterArray)
	{
		delete[] _characterArray;
		_characterArray = nullptr;
	}

	_length = static_cast<unsigned short>(stdStr.length() + 1);
	_characterArray = new char[stdStr.length() + 1];
	memcpy_s(_characterArray, _length, stdStr.c_str(), stdStr.length());
	_characterArray[_length - 1] = '\0';
}

const char* CNetString::CStr() const
{
	return _characterArray;
}

void CNetString::Serialize(INetMessageWriter& serializer) const
{
	serializer.Write(_length);
	serializer.Write(_characterArray, sizeof(char) * _length);
}

void CNetString::Deserialize(INetMessageReader& serializer)
{
	if (_characterArray)
	{
		delete[] _characterArray;
	}

	serializer.Read(_length);
	_characterArray = new char[_length];
	serializer.Read(_characterArray, sizeof(char) * _length);
}
