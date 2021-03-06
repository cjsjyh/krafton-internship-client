#pragma once
#ifndef _STATICOBJECTCLASS_H_
#define _STATICOBJECTCLASS_H_

class gameObject;
class D3DClass;
class ModelClass;

class staticobjclass : public gameObject
{
public:
	staticobjclass(string objname, D3DClass* _device, CollisionChannel _channel = PLAYER, ColliderType _col=COLLIDER_BOX);
	~staticobjclass();
	
	void InitializeStatic2D(string prefix="");
	void InitializeStatic2DItem(string prefix="");
	void InitializeStatic2DItem(string prefix, string fname);
	void InitializeStatic3D(string prefix = "");
	bool CheckDestroy();
private:
	
};

#endif