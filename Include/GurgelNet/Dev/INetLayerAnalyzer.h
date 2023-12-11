#pragma once

class INetLayerAnalyzer
{
public:
	virtual void NetDataSend(unsigned int numBytes) = 0;
	
	virtual void NetDataRecieve(unsigned int numBytes) = 0;

	virtual void UpdateConnectionCount(unsigned int newCount) = 0;

	virtual void UpdateNetObjectCount(unsigned int newCount) = 0;
};