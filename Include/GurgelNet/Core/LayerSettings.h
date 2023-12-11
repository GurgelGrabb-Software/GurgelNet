// GurgelNet - 2023
// ------------------------------------------------------------
// LayerSettings.h 
// ------------------------------------------------------------
// ------------------------------------------------------------

#pragma once

#include <string>
#include "GurgelNet/LayerCallbacks.h"
#include "GurgelNet/Objects/INetObjectFactory.h"
#include "GurgelNet/Dev/INetLayerAnalyzer.h"

struct SNetLayerSettings
{
	SNetLayerSettings();

	/// <summary>
	/// Server: Unused
	/// Client: IP to connect to. If empty, attempt connection to localhost
	/// </summary>
	std::string ip; 

	/// <summary>
	/// Server: The port to listen to
	/// Client: The port on the server to send messages to
	/// </summary>
	unsigned short port;

	/// <summary>
	/// Buffer of layer callbacks.
	/// </summary>
	void* layerCallbacks[ENetLayerCallback_Count];

	/// <summary>
	/// Pointer to an object factory to allow networked construction of objects.
	/// </summary>
	INetObjectFactory* objectFactoryPtr;

	/// <summary>
	/// Optional pointer to analyzer implementation
	/// </summary>
	INetLayerAnalyzer* analyzerPtr;
};