#include "GurgelNet/Objects/NetworkVariable.h"

#include "Src/Core/Logging.h"

CNetworkVariable::CNetworkVariable()
	: _netVarID(NetVarID_Unset)
	, _ownerMask(ClientID_Server) // Default to server ownership always
	, _isOwner(false)
	, _lastSync()
	, _syncRate(0.0f)
	, _dirty(false)
{
}

CNetworkVariable::CNetworkVariable(ClientID ownerMask)
	: _netVarID(NetVarID_Unset)
	, _ownerMask(ownerMask)
	, _isOwner(false)
	, _lastSync()
	, _syncRate(0.0f)
	, _dirty(false)
{
}

CNetworkVariable::CNetworkVariable(ClientID ownerMask, float syncRate)
	: _netVarID(NetVarID_Unset)
	, _ownerMask(ownerMask)
	, _isOwner(false)
	, _lastSync()
	, _syncRate(syncRate)
	, _dirty(false)
{
}

void CNetworkVariable::Initialize(NetVarID id, bool asOwner)
{
	_isOwner = asOwner;
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

void CNetworkVariable::SetOwner(ClientID ownerMask)
{
	_ownerMask = ownerMask;
}

ClientID CNetworkVariable::GetOwnerMask() const
{
	return _ownerMask;
}

bool CNetworkVariable::IsOwner() const
{
	return _isOwner;
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

bool CNetworkVariable::TryWrite()
{
	const bool r = IsOwner();

	if (!r)
	{
		NET_LOG(ENetLogLevel_Warning, "Tried writing to network variable that was not owned");
	}

	return r;
}
