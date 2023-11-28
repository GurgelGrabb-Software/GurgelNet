#include "Src/Messages/NetMessage.h"
#include "GurgelNet/Serialization/INetSerializer.h"
#include "GurgelNet/Messages/NetMessageHeader.h"

#include <cstring>
#include <new>

SNetMessage::SNetMessage()
	: pData(nullptr)
	, nBytes(0u)
	, reliable(false)
{
}

SNetMessage::SNetMessage(void* rPtr, size_t s)
	: pData(nullptr)
	, nBytes(s)
	, reliable(false)
{
	uint8_t* cpyByteArray = new uint8_t[s];
	pData = cpyByteArray;
	memcpy_s(pData, nBytes, rPtr, nBytes);
}

SNetMessage::SNetMessage(SNetMessage&& o) noexcept
	: pData(o.pData)
	, nBytes(o.nBytes)
	, reliable(o.reliable)
{
	o.pData = nullptr;
	o.nBytes = 0u;
}

SNetMessage::~SNetMessage()
{
	Release();
}

SNetMessage::operator bool() const
{
	return pData != nullptr;
}

bool SNetMessage::UseDefaultTarget() const
{
	return ExtractHeader().targetID == 0;
}

bool SNetMessage::ShouldSendTo(uint8_t id) const
{
	return (ExtractHeader().targetID & id) != 0;
}

void SNetMessage::SetSender(uint8_t senderID)
{
	ExtractHeader().senderID = senderID;
}

void SNetMessage::SetTarget(uint8_t targetMask)
{
	ExtractHeader().targetID = targetMask;
}

void SNetMessage::Package(INetMessageWriter& writer, uint8_t primaryTypeID, uint8_t userData)
{
	const size_t headerSize = sizeof(SNetMessageHeader);
	const size_t payloadSize = writer.Size();
	const size_t byteSize = headerSize + payloadSize;

	uint8_t* byteArray = new uint8_t[byteSize];
	memset(byteArray, 0x00, byteSize);
	pData = byteArray;
	nBytes = byteSize;

	SNetMessageHeader& header = ExtractHeader();
	header.categoryID = primaryTypeID;
	header.subTypeID = userData;

	writer.Pack(&byteArray[headerSize]);
}

void SNetMessage::Release()
{
	if (pData)
	{
		delete[nBytes] pData;
	}
}

SNetMessageHeader& SNetMessage::ExtractHeader()
{
	return *(SNetMessageHeader*)(pData);
}

const SNetMessageHeader& SNetMessage::ExtractHeader() const
{
	return *(SNetMessageHeader*)(pData);
}
