#pragma once
#include <string>
#include "GurgelNet/LayerCallbacks.h"
#include "GurgelNet/Objects/INetObjectFactory.h"

struct SNetLayerCallbackHandle
{
	SNetLayerCallbackHandle() : callbackType(ENetLayerCallback_Count), callbackPtr(nullptr) {}
	SNetLayerCallbackHandle(ENetLayerCallback t, void* fPtr) : callbackType(t), callbackPtr(fPtr) {}

	ENetLayerCallback callbackType;
	void* callbackPtr;
};

struct SNetLayerSettings
{
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
	SNetLayerCallbackHandle layerCallbacks[ENetLayerCallback_Count];

	/// <summary>
	/// Pointer to an object factory to allow networked construction of objects.
	/// </summary>
	INetObjectFactory* objectFactoryPtr;
};