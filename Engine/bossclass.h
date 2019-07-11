#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "gameObject.h"

class bossclass : public gameObject
{
public:
	bossclass(ModelClass* model, int _hp, int _damage, ColliderType col = COLLIDER_BOX);
	~bossclass();

	void Move();
	bool CheckDestroy();
	void Hit(int damage);
private:
	int hp;
	int phase;
	int damage;
};

#endif

