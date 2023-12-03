#pragma once
#include "GurgelNet/LayerCallbacks.h"

class CNetLayerCallbackStore
{
public:
	CNetLayerCallbackStore();

	// ------------------------------------------------------------
	// Registration
	// ------------------------------------------------------------

	void RegisterCallback(ENetLayerCallback cbType, void* cbPtr);
	void RegisterCallbacks(void* cbArr[ENetLayerCallback_Count]);

	// ------------------------------------------------------------
	// Getters
	// ------------------------------------------------------------
	
	template<typename TFPtr>
	TFPtr GetCallbackAs(ENetLayerCallback cbType) { return (TFPtr)GetCallback(cbType); }
	void* GetCallback(ENetLayerCallback cbType);

	bool HasRegisteredCallback(ENetLayerCallback cbType) const;

private:
	void* _callbacks[ENetLayerCallback_Count];
};