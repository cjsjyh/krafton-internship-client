#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "gameObject.h"

class bossclass : public gameObject
{
public:
	bossclass(ModelClass* model, ColliderType col, int _hp, int _damage);
	~bossclass();

	void Move();
	bool CheckDestroy();
private:
	int hp;
	int phase;
	int damage;
};

#endif

