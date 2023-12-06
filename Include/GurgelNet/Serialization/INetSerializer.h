// GurgelNet - 2023
// ------------------------------------------------------------
// NetSerializer.h 
// ------------------------------------------------------------
// Interfaces for message serialization.
// ------------------------------------------------------------

#pragma once

#include <type_traits>
#include <concepts>

// ------------------------------------------------------------

template<typename T>
concept TriviallyCopyable = std::is_trivially_copyable<T>::value;

class INetSerializable;

// ------------------------------------------------------------

class INetMessageSerializer
{
public:
	template< TriviallyCopyable T >
	void Serialize(T& v)
	{
		Serialize(&v, sizeof(T));
	}

	virtual void Serialize(void* ptr, size_t s) = 0;
	virtual void Serialize(INetSerializable& serializable) = 0;
};

// ------------------------------------------------------------

class INetMessageWriter : public INetMessageSerializer
{
public:
	template<TriviallyCopyable T>
	void Write(const T& v)
	{
		Write(&v, sizeof(T));
	}

	virtual void Write(const void* ptr, size_t s) = 0;
	virtual void Write(const INetSerializable& serializable) = 0;

	virtual const void* RawData() const = 0;
	virtual size_t Size() const = 0;
	virtual void Pack(void* target) = 0;
};

// ------------------------------------------------------------

class INetMessageReader : public INetMessageSerializer
{
public:
	template<TriviallyCopyable T>
	void Read(T& v)
	{
		Read(&v, sizeof(T));
	}

	template<TriviallyCopyable T>
	T Read()
	{
		T temp;
		Read<T>(temp);
		return temp;
	}

	virtual void Read(void* ptr, size_t s) = 0;
	virtual void Read(INetSerializable& serializable) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------