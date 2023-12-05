# GurgelNet 

GurgelNet is a simple networking library built on top of [Valve's GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets/)

# Features
* Object System
  * Base type of `CNetObject` that allows you to still have custom object types 
  * Late join synchronization of objects spawned
  * Network Variables for easy sending and syncing of data to specific object instances
  * Ownership system controlling who can write vs read specific data
* Sending of custom network data to allow game-specific cases

# Getting Started
All of your initial interfacing with GurgelNet is done through the `GurgelNet_` functions found in `GurgelNet/Network.h`.

## Initializing GurgelNet
The first step to start using GurgelNet is to initialize the library.
```cpp
#include <GurgelNet/Network.h>

void Init()
{
  GurgelNet_Init(nullptr, 0);
}
```

## Creating Net Layer(s)
You can now create your Net Layers. 
A Net Layer represents a connection to the network, either as a server or as a client.

To create a Net Layer, you will have to pass an object of type `SNetLayerSettings`.  
```cpp
#include <GurgelNet/Network.h>

HNetLayer clientHandle;

bool CreateClient( std::string ip, unsigned short port )
{
  SNetLayerSettings settings;
  settings.ip = ip;                 // The IP to connect to (here, localhost) - Not required for servers
  settings.port = port;             // The port to connect to
  settings.objectFactoryPtr = ...;  // The factory ptr will be covered later

  return GurgelNet_CreateClient(settings, clientHandle))
}
```

To create a server, it's a similar procedure where you instead call `GurgelNet_CreateServer`.  
_Servers do not require you to set an IP_

## Starting & Running the networking
Now that you have created a layer, all you have to do is to Start and Tick the networking.

```cpp
void Run()
{
  GurgelNet_Start();

  while(running)
  {
    GurgelNet_Tick();
  }

  GurgelNet_Shutdown();
}
```

# Customizing GurgelNet

## The `INetObjectFactory`
This interface is to allow you to hook up your own TypeID to Object instantiation (and create objects through your own object system).

As a very basic example:
```cpp
class CExampleObjectFactory : public INetObjectFactory
{
  CNetObject* MakeObject(NetTypeID typeID) override
  {
    switch(typeID)
    {
    case 1: return CObjectTypeOne();  // Derived type from CNetObject
    case 2: return CObjectTypeTwo();  // Derived type from CNetObject
    }

    return nullptr;
  }

  // In case you want to run any pre-networking initialization
  void PreSpawn(CNetObject& object) override {}

  // In case you want to run any post-networking initialization
  void PostSpawn(CNetObject& object) override {}
}
```
