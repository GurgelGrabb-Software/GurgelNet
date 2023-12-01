#include "GurgelNet/Network.h"

#include "Src/Core/Logging.h"
#include "Src/BaseNetLayer.h"
#include "Src/ClientLayer.h"
#include "Src/ServerLayer.h"

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#pragma comment(lib, "GameNetworkingSockets")

static CNetLayerBase* s_layerPtrs[3]; 
static size_t s_numLayers;


void GurgelNet_Init(SNetLibSetting* pSettings, int nSettings)
{
	s_layerPtrs[0] = nullptr;
	s_layerPtrs[1] = nullptr;
	s_layerPtrs[2] = nullptr;
	s_numLayers = 1;

	for (int i = 0; i < nSettings; ++i)
	{
		const SNetLibSetting& current = pSettings[i];

		switch (current.type)
		{
		case ENetLibSetting_Callback_Log: 
			CNetLogger::_logCallback = (FNetLibLogCallback)(current.data.s_ptr);
			break;
		case ENetLibSetting_Multithread:
			if (current.data.s_flag)
				NET_LOG(ENetLogLevel_Warning, "Multithread was enabled but is not yet implemented");
			break;

		default:
			break;
		}

	}

	SteamNetworkingIdentity id;
	id.Clear();

	SteamNetworkingErrMsg errorMsg;
	if (!GameNetworkingSockets_Init(&id, errorMsg))
	{
		NET_LOG(ENetLogLevel_Error, "Failed to init net library. Message: '{}'", errorMsg);
	}
}

INetLayer* GurgelNet_GetLayer(HNetLayer handle)
{
	return s_layerPtrs[handle];
}

void GurgelNet_Start()
{
	for (CNetLayerBase* l : s_layerPtrs)
	{
		if (l != nullptr)
		{
			l->Start();
		}
	}
}

void GurgelNet_Tick()
{
	SteamNetworkingSockets()->RunCallbacks();
	for (size_t i = 1; i < s_numLayers; ++i)
	{
		if (s_layerPtrs[i])
		{
			s_layerPtrs[i]->Tick();
		}
	}
}

void GurgelNet_Shutdown()
{
	GameNetworkingSockets_Kill();

	for (CNetLayerBase* l : s_layerPtrs)
	{
		if (l != nullptr)
		{
			delete l;
			l = nullptr;
		}
	}

	s_numLayers = 1;
}

void AssignLayerSettings(CNetLayerBase& layer, const SNetLayerSettings& settings)
{
	for (auto& callback : settings.layerCallbacks)
	{
		if (callback.callbackPtr == nullptr) continue;
		layer.SetLayerCallback(callback.callbackType, callback.callbackPtr);
	}

	layer.RegisterObjectFactory(*settings.objectFactoryPtr);
}

bool GurgelNet_CreateClient(const SNetLayerSettings& clientSettings, HNetLayer& outHandle)
{
	outHandle = 0;

	if (s_numLayers == 3)
	{
		NET_LOG(ENetLogLevel_Error, "Cannot create more than 2 net layers");
		return false;
	}

	s_layerPtrs[s_numLayers] = new CClientLayer(clientSettings.ip, clientSettings.port);
	AssignLayerSettings(*s_layerPtrs[s_numLayers], clientSettings);
	outHandle = s_numLayers;
	s_numLayers++;

	return true;
}

bool GurgelNet_CreateServer(const SNetLayerSettings& serverSettings, HNetLayer& outHandle)
{
	outHandle = 0;

	if (s_numLayers == 3)
	{
		NET_LOG(ENetLogLevel_Error, "Cannot create more than 2 net layers");
		return false;
	}

	s_layerPtrs[s_numLayers] = new CServerLayer(serverSettings.port);
	AssignLayerSettings(*s_layerPtrs[s_numLayers], serverSettings);
	outHandle = s_numLayers;
	s_numLayers++;
	return true;
}

bool GurgelNet_CreateHost(const SNetLayerSettings& serverSettings, const SNetLayerSettings& clientSettings, HNetLayer& outClientHandle, HNetLayer& outServerHandle)
{
	bool result = true;
	result &= GurgelNet_CreateServer(serverSettings, outServerHandle);
	result &= GurgelNet_CreateClient(clientSettings, outClientHandle);
	return result;
}
