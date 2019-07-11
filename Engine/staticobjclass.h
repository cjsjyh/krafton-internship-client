#pragma once
#ifndef _STATICOBJECTCLASS_H_
#define _STATICOBJECTCLASS_H_

#include "gameObject.h"


class staticobjclass : public gameObject
{
public:
	staticobjclass(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel);
	~staticobjclass();
	
	bool CheckDestroy();
private:
};

#endif

