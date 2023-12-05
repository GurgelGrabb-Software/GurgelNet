#pragma once
#include "GurgelNet/Objects/NetworkVariable.h"

#include <vector>

class CNetVarList
{
public:
	CNetVarList();
	CNetVarList(const CNetVarList&) = delete;
	CNetVarList(CNetVarList&& o) noexcept;

	void operator=(CNetVarList&& o) noexcept;
	NetVarID Num() const;
	void Clear();

	CNetworkVariable* GetVariable(NetVarID id);
	const CNetworkVariable* GetVariable(NetVarID id) const;

	void RegisterVariable(CNetworkVariable& var, bool asOwner);

	void WriteNetVarStates(INetMessageWriter& serializer);
	void ReadNetVarStates(INetMessageReader& serializer);

private:
	std::vector<CNetworkVariable*> _variables;
};