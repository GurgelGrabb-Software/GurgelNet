// GurgelNet - 2023
// ------------------------------------------------------------
// Network.h 
// ------------------------------------------------------------
// Core library initialization and types
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/INetLayer.h"
#include "GurgelNet/Core/LibrarySettings.h"
#include "GurgelNet/Core/NetLogging.h"
#include "GurgelNet/Core/NetTypes.h"
#include "GurgelNet/Core/LayerSettings.h"

extern "C"
{
	/// <summary>
	/// Initialize the library to make it ready for use. Must be called before Start or Tick.
	/// </summary>
	/// <param name="pSettings">Pointer to settings</param>
	/// <param name="nSettings">Num settings</param>
	NETWORK_API void GurgelNet_Init(SNetLibSetting* pSettings, int nSettings);

	/// <summary>
	/// Shut down and clean up resources used by the library
	/// </summary>
	NETWORK_API void GurgelNet_Shutdown();

	/// <summary>
	/// Start any created network layers (See _CreateClient, _CreateServer and _CreateHost)
	/// </summary>
	NETWORK_API void GurgelNet_Start();

	/// <summary>
	/// Tick all net layers. You should run this once per frame (assuming single threaded execution)
	/// </summary>
	NETWORK_API void GurgelNet_Tick();
	
	/// <summary>
	/// Get the network layer with a given handle
	/// </summary>
	/// <param name="handle">The handle to the network layer to get</param>
	/// <returns>INetLayer* to the layer of given handle, or nullptr if handle is invalid</returns>
	NETWORK_API INetLayer* GurgelNet_GetLayer(HNetLayer handle);

	/// <summary>
	/// Create a client that will try to connect to the given server data on start
	/// </summary>
	/// <param name="ip">Ip of the server</param>
	/// <param name="port">Port of the server</param>
	/// <param name="outHandle">Handle to the client layer</param>
	/// <returns>true on sucess, otherwise false</returns>
	NETWORK_API bool GurgelNet_CreateClient(SNetLayerSettings& clientSettings, HNetLayer& outHandle);

	/// <summary>
	/// Create a server that will start listening on the local port on start
	/// </summary>
	/// <param name="port">Port to listen on</param>
	/// <param name="outHandle">Handle to the server layer</param>
	/// <returns>true on sucess, otherwise false</returns>
	NETWORK_API bool GurgelNet_CreateServer(SNetLayerSettings& serverSettings, HNetLayer& outHandle);

	/// <summary>
	/// Utility to create a server and a client that connects to localhost
	/// </summary>
	/// <param name="port">Port for server to listen to</param>
	/// <param name="outClientHandle">Handle to the created client layer</param>
	/// <param name="outServerHandle">Handle to the created server layer</param>
	/// <returns>true on success, otherwise false</returns>
	NETWORK_API bool GurgelNet_CreateHost(SNetLayerSettings& serverSettings, SNetLayerSettings& clientSettings, HNetLayer& outClientHandle, HNetLayer& outServerHandle);
}

