#pragma once
#include "GurgelNet/Core/NetTypes.h"
#include "Src/Include/NetLayers/NetLayerContext.h"
#include "GurgelNet/Objects/NetObjectOwnership.h"

class INetObjectFactory;
class CNetworkVariable;
class CNetObject;
struct SNetObjectHandle;


class CSharedObjectHandler
{
public:
	CSharedObjectHandler(SNetLayerContext& netContext);
	void AssignObjectFactory(INetObjectFactory* factoryPtr);

	virtual void SpawnObject(CNetObject& object, ENetObjectOwner owner) = 0;
	virtual void DespawnObject(CNetObject& object) = 0;

	virtual void RunNetObjectUpdate() = 0;

protected:
	SNetLayerContext& _netContext;

	INetObjectFactory* ObjectFactory();

	void UpdateNetObject(SNetObjectHandle& handle);
	void UpdateNetVar(NetObjectID objectID, CNetworkVariable& var);

private:
	INetObjectFactory* _factoryPtr;

};