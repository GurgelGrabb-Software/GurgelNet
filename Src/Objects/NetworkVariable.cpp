#include "GurgelNet/Objects/NetworkVariable.h"

CNetworkVariable::CNetworkVariable()
	: _lastSync()
	, _syncRate(0.0f)
	, _dirty(false)
{
}

CNetworkVariable::CNetworkVariable(float syncRate)
	: _lastSync()
	, _syncRate(syncRate)
	, _dirty(false)
{
}

void CNetworkVariable::Initialize(NetVarID id)
{
	_netVarID = id;
}

bool CNetworkVariable::IsDirty() const
{
	return _dirty;
}

using secDuration = std::chrono::duration<float>;
bool CNetworkVariable::ShouldSync() const
{
	auto durSinceSync = std::chrono::high_resolution_clock::now() - _lastSync;
	auto secSinceSync = std::chrono::duration_cast<secDuration>(durSinceSync);
	return _dirty && secSinceSync.count() >= _syncRate;
}

void CNetworkVariable::MarkSynced()
{
	_lastSync = std::chrono::high_resolution_clock::now();
	_dirty = false;
}

NetVarID CNetworkVariable::GetVariableID() const
{
	return _netVarID;
}

void CNetworkVariable::Serialize(INetMessageWriter& serializer) const
{
}

void CNetworkVariable::Deserialize(INetMessageReader& serializer)
{
}
