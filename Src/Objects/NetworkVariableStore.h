#pragma once
#include <vector>

class CNetworkVariable;

class CNetworkVariableStore
{
public:
	void RegisterVariable(CNetworkVariable& variable);

private:
	std::vector<CNetworkVariable*> _variables;
};