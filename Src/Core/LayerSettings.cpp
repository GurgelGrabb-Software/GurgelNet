#include "GurgelNet/Core/LayerSettings.h"

SNetLayerSettings::SNetLayerSettings()
	: ip()
	, port(0)
	, layerCallbacks()
	, objectFactoryPtr(nullptr)
{
	for (void* p : layerCallbacks)
	{
		p = nullptr;
	}
}
