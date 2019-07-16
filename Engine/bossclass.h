#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"

class projectileclass;
class playerclass;
class D3DClass;
class gameObject;

class bossclass : public hpobjects
{
public:
	bossclass(int _hp, int _damage, D3DClass*, playerclass*,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	vector<projectileclass*> Frame(int);
	

private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;

	projectileclass* Fire();
	void Fire8Direction();

	void PushQueue(projectileclass*);
	void CheckQueue();
	void PopQueue();

	vector<projectileclass*> bossBullets;
};

#endif

