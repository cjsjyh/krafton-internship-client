#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"

class bossclass : public hpobjects
{
public:
	bossclass(ModelClass* model, int _hp, int _damage, ColliderType col = COLLIDER_BOX);
	~bossclass();

	void Move();
private:
	int phase;
	int damage;
};

#endif

