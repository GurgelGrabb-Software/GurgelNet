# GurgelNet 

GurgelNet is a simple networking library built on top of [Valve's GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets/)

## Getting Started

All main interfacing with the library is done through the `GurgelNet/Network.h` header and the library functions within it.

Example setup to get a simple Host (client + server) running:
```cpp
#include <GurgelNet/Network.h>

void Run()
{
  bool running = true;

  // The parameters here are a pointer/array of network settings and the amount of settings (here = 0)
  GurgelNet_Init(nullptr, 0);

  const unsigned short port = 25565;
  NetLayer_h server_h;
  NetLayer_h client_h;

  // _CreateHost creates a server and a client connected to local host
  // The server will listen to the provided port (here 25565)
  // You can use the handles to access the INetLayer of each part to do more custom setup
  running = GurgelNet_CreateHost(port, client_h, server_h);

  if(running)
  {
    // _Start will start running all created NetworkLayers
    GurgelNet_Start();
  }
  while(running)
  {
    // _Tick will run message receiving, processing and sending for each created NetworkLayer
    GurgelNet_Tick();
  }

  // Shutdown is a simple function call that cleans up the full library
  GurgelNet_Shutdown();
}
```
