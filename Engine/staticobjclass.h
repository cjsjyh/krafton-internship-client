#pragma once
#ifndef _STATICOBJECTCLASS_H_
#define _STATICOBJECTCLASS_H_

class gameObject;
class D3DClass;
class ModelClass;

class staticobjclass : public gameObject
{
public:
	staticobjclass(string objname, D3DClass* _device, ColliderType _col=COLLIDER_BOX, CollisionChannel _channel=HIT_PLAYER);
	~staticobjclass();
	
	void InitializeStatic();
	bool CheckDestroy();
private:
	
};

#endif

