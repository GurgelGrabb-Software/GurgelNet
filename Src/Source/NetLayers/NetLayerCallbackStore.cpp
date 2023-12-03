#include "Src/Include/NetLayers/NetLayerCallbackStore.h"

// ------------------------------------------------------------

CNetLayerCallbackStore::CNetLayerCallbackStore()
	: _callbacks()
{
	for (unsigned int i = 0; i < ENetLayerCallback_Count; ++i)
	{
		_callbacks[i] = nullptr;
	}
}

// ------------------------------------------------------------
// Registration
// ------------------------------------------------------------

void CNetLayerCallbackStore::RegisterCallback(ENetLayerCallback cbType, void* cbPtr)
{
	_callbacks[cbType] = cbPtr;
}

// ------------------------------------------------------------

void CNetLayerCallbackStore::RegisterCallbacks(void* cbArr[ENetLayerCallback_Count])
{
	for (int i = 0; i < ENetLayerCallback_Count; ++i)
	{
		RegisterCallback((ENetLayerCallback)i, cbArr[i]);
	}
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------

void* CNetLayerCallbackStore::GetCallback(ENetLayerCallback cbType)
{
	return _callbacks[cbType];
}

// ------------------------------------------------------------

bool CNetLayerCallbackStore::HasRegisteredCallback(ENetLayerCallback cbType) const
{
	return _callbacks[cbType] != nullptr;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------