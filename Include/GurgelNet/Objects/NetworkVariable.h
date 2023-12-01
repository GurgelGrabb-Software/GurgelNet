#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Serialization/INetSerializable.h"
#include "GurgelNet/Serialization/INetSerializer.h"

#include <functional>
#include <chrono>

class CNetObject;

class CNetworkVariable : public INetSerializable
{
public:
	CNetworkVariable();
	CNetworkVariable(float syncRate);
	virtual ~CNetworkVariable() = default;
	
	void Initialize(NetVarID id);
	bool IsDirty() const;
	bool ShouldSync() const;
	virtual bool SyncReliable() const = 0;
	void MarkSynced();

	NetVarID GetVariableID() const;

	virtual void Serialize(INetMessageWriter& serializer) const override;
	virtual void Deserialize(INetMessageReader& serializer) override;

private:
	NetVarID _netVarID;
	std::chrono::high_resolution_clock::time_point _lastSync;
	float _syncRate;

protected:
	bool _dirty;
};

// Template utility

template<typename T, bool TReliable = false>
class TNetworkVariable : public CNetworkVariable
{
public:
	TNetworkVariable() : CNetworkVariable() {}
	TNetworkVariable(float syncRate) : CNetworkVariable(syncRate) {}
	TNetworkVariable(const T& v) : CNetworkVariable(), _value(v) {}
	TNetworkVariable(const T& v, float syncRate) : CNetworkVariable(syncRate), _value(v) {}

	bool SyncReliable() const override { return TReliable; }
	void Serialize(INetMessageWriter& serializer) const override { serializer.Write(_value); }
	void Deserialize(INetMessageReader& serializer) override { serializer.Read(_value); }

	void operator=(const T& rhs) { _value = rhs; _dirty = true; }
	const T& Value() const { return _value; }
private:
	T _value;
};