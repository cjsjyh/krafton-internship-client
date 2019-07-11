#pragma once
#ifndef _HPOBJECTS_H_
#define _HPOBJECTS_H_

#include "gameObject.h"

class hpobjects : public gameObject
{
public:
	hpobjects(string, ModelClass*, int, CollisionChannel, ColliderType = COLLIDER_BOX);
	void Hit(int);
	bool CheckDestroy();

protected:
	int hp;
};

#endif

