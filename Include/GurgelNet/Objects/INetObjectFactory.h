// GurgelNet - 2023
// ------------------------------------------------------------
// INetObjectFactory.h 
// ------------------------------------------------------------
// Interface to allow constructing objects remotely via a
// NetTypeID
// 
// You should register this with your network layer to allow
// constructing network objects using your own allocator
// ------------------------------------------------------------

#pragma once

#include "GurgelNet/NetLib.h"
#include "GurgelNet/Objects/NetObject.h"

// ------------------------------------------------------------

class INetObjectFactory
{
public:
	/// <summary>
	/// Called to construct an object.
	/// Note that GurgelNet will assume that the object remains in place in memory after creation.
	/// </summary>
	/// <param name="id">The net safe type ID (up to you to interpret and process)</param>
	/// <returns>Pointer to your newly created object</returns>
	virtual CNetObject* MakeObject( NetTypeID id ) = 0;
	
	/// <summary>
	/// Called before the spawning logic is run
	/// Use this to do any pre-net work such as creation logic that needs to exist when spawning
	/// </summary>
	/// <param name="object">The object</param>
	virtual void PreSpawn(CNetObject& object) = 0;

	/// <summary>
	/// Called after the spawning logic has run
	/// Use this to do any final initialization that requires the networking to be valid
	/// </summary>
	/// <param name="object">The object</param>
	virtual void PostSpawn(CNetObject& object) = 0;
};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------