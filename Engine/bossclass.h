#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"
#include "projectileclass.h"
#include "playerclass.h"

class bossclass : public hpobjects
{
public:
	bossclass(int _hp, int _damage, D3DClass*, playerclass*,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	projectileclass* Fire();
	

private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;
};

#endif

