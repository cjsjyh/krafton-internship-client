#pragma once
#ifndef _STATICOBJECTCLASS_H_
#define _STATICOBJECTCLASS_H_

#include "gameObject.h"


class staticobjclass : public gameObject
{
public:
	staticobjclass(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel);
	~staticobjclass();
	
	void Move();
	bool CheckDestroy();
private:
};

#endif

