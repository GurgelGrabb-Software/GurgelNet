#include "Src/Include/Objects/NetVarList.h"

CNetVarList::CNetVarList()
{
}

CNetVarList::CNetVarList(CNetVarList&& o) noexcept
	: _variables(std::move(o._variables))
{
}

void CNetVarList::operator=(CNetVarList&& o) noexcept
{
	_variables = std::move(o._variables);
}

NetVarID CNetVarList::Num() const
{
	return (NetVarID)_variables.size();
}

void CNetVarList::Clear()
{
	_variables.clear();
}

CNetworkVariable* CNetVarList::GetVariable(NetVarID id)
{
	return _variables[id];
}

const CNetworkVariable* CNetVarList::GetVariable(NetVarID id) const
{
	return _variables[id];
}

void CNetVarList::RegisterVariable(CNetworkVariable& var)
{
	const NetVarID id = (NetVarID)_variables.size();
	_variables.push_back(&var);
	var.Initialize(id);
}

void CNetVarList::WriteNetVarStates(INetMessageWriter& serializer)
{
	for (auto varPtr : _variables)
	{
		serializer.Write(*varPtr);
	}
}

void CNetVarList::ReadNetVarStates(INetMessageReader& serializer)
{
	for (auto varPtr : _variables)
	{
		serializer.Read(*varPtr);
	}
}
