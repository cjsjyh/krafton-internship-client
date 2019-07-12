#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"
#include "projectile.h"
#include "playerclass.h"

class bossclass : public hpobjects
{
public:
	bossclass(int _hp, int _damage, D3DClass*, playerclass*,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	projectile* Fire();
	

private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;
};

#endif

