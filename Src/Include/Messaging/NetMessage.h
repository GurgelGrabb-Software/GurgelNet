#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetTypes.h"

// ------------------------------------------------------------

struct SNetMessageHeader;
class INetMessageWriter;

// ------------------------------------------------------------

struct SNetMessage
{
public:
	SNetMessage();
	SNetMessage(void* rPtr, size_t s);
	SNetMessage(SNetMessage&& o) noexcept;
	SNetMessage(const SNetMessage&) = delete;
	~SNetMessage();

	operator bool() const;

	bool UseDefaultTarget() const;

	uint8_t GetCategory() const;
	ClientID GetSenderID() const;
	ClientID GetTargetMask() const;

	void SetSender(ClientID senderID);
	void SetTarget(ClientID targetMask);

	void Package(INetMessageWriter& writer, uint8_t primaryTypeID, uint8_t userData = 0u);
	void Release();

	void* pData;
	size_t nBytes;
	bool reliable;

private:
	SNetMessageHeader& ExtractHeader();
	const SNetMessageHeader& ExtractHeader() const;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------