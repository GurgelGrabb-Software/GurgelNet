#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Dev/INetLayerAnalyzer.h"

class ISteamNetworkingSockets;
class CNetLayerCallbackStore;
class INetMessageQueue;

struct SNetLayerContextBackendData
{
	ISteamNetworkingSockets* interfacePtr;
	unsigned int hConnection;
	unsigned int hPollGroup;
};

struct SNetLayerContextLayerData
{
	ClientID layerNetworkID;
	INetMessageQueue* msgQueuePtr;
	CNetLayerCallbackStore* callbackStorePtr;
};

struct SNetLayerContext
{
	SNetLayerContextLayerData layer;
	SNetLayerContextBackendData backend;
	INetLayerAnalyzer* analyzerPtr;
};