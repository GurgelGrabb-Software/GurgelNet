#pragma once
#include "GurgelNet/NetLib.h"
#include <cstdint>

struct SNetMessageHeader;
class INetMessageWriter;

struct NETWORK_API SNetMessage
{
public:
	SNetMessage();
	SNetMessage(void* rPtr, size_t s);
	SNetMessage(SNetMessage&& o) noexcept;
	SNetMessage(const SNetMessage&) = delete;
	~SNetMessage();

	operator bool() const;

	bool UseDefaultTarget() const;
	bool ShouldSendTo(uint8_t id) const;

	void SetSender(uint8_t senderID);
	void SetTarget(uint8_t targetMask);

	void Package(INetMessageWriter& writer, uint8_t primaryTypeID, uint8_t userData = 0u);
	void Release();

	void* pData;
	size_t nBytes;
	bool reliable;

private:
	SNetMessageHeader& ExtractHeader();
	const SNetMessageHeader& ExtractHeader() const;
};
