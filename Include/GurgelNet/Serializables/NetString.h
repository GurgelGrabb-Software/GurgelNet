#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Serialization/INetSerializable.h"
#include <string>

class NETWORK_API CNetString : public INetSerializable
{
public:
	CNetString();
	CNetString(CNetString&& netStr) noexcept;
	CNetString(const CNetString& netStr);
	
	CNetString(const char* cStr, unsigned short len);
	CNetString(const std::string& stdStr);

	~CNetString();

	void operator=(const std::string& stdStr);

	const char* CStr() const;

	void Serialize(INetMessageWriter& serializer) const override;
	void Deserialize(INetMessageReader& serializer) override;

private:
	unsigned short _length;
	char* _characterArray;
};