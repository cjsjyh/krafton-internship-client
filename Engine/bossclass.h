#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"
#include "projectile.h"
#include "playerclass.h"

class bossclass : public hpobjects
{
public:
	bossclass(ModelClass*, ModelClass*, playerclass*, int _hp, int _damage, ColliderType col = COLLIDER_BOX);
	~bossclass();

	projectile* Fire();
	D3DXVECTOR3 normalizeVec3(D3DXVECTOR3);

private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;
};

#endif

