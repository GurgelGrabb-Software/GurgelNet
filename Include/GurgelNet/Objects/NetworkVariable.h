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
	CNetworkVariable(ClientID ownerMask);
	CNetworkVariable(ClientID ownerMask, float syncRate);
	virtual ~CNetworkVariable() = default;
	
	void Initialize(NetVarID id);
	bool IsDirty() const;
	bool ShouldSync() const;
	virtual bool SyncReliable() const = 0;
	void MarkSynced();

	bool IsOwner(ClientID id) const;
	NetVarID GetVariableID() const;

	virtual void Serialize(INetMessageWriter& serializer) const override;
	virtual void Deserialize(INetMessageReader& serializer) override;

private:
	NetVarID _netVarID;
	ClientID _ownerMask;

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
	TNetworkVariable(ClientID ownerMask) : CNetworkVariable(ownerMask) {}
	TNetworkVariable(ClientID ownerMask, float syncRate) : CNetworkVariable(ownerMask, syncRate) {}
	TNetworkVariable(const T& v, ClientID ownerMask = ClientID_Server) : CNetworkVariable(ownerMask), _value(v) {}
	TNetworkVariable(const T& v, float syncRate, ClientID ownerMask = ClientID_Server) : CNetworkVariable(ownerMask, syncRate), _value(v) {}

	void BindOnValueChanged(std::function<void(const T&, const T&)> func) { _onValChange = func; }

	bool SyncReliable() const override { return TReliable; }
	void Serialize(INetMessageWriter& serializer) const override { serializer.Write(_value); }
	void Deserialize(INetMessageReader& serializer) override 
	{ 
		const T oldVal = _value;
		serializer.Read(_value);
		if (_onValChange)
		{
			_onValChange(oldVal, _value);
		}
	}

	void operator=(const T& rhs) { _value = rhs; _dirty = true; }
	const T& Value() const { return _value; }
private:
	T _value;
	std::function<void(const T&, const T&)> _onValChange;
};