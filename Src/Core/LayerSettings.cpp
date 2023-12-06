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

#include <functional>

struct FTest
{
	FTest(std::function<void()> f)
	{
	}
};

struct STest
{
	void F()
	{
	}

	FTest t = FTest([&]() {F(); });
};