#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameObject.h"
class ModelClass;
class D3DClass;

class projectileclass : public gameObject
{
public:
	projectileclass(string, D3DXVECTOR3, float _speed, int _damage, D3DClass*, int delay = 20, int _distance=100, gameObject::CollisionChannel = HIT_PLAYER, gameObject::ColliderType = COLLIDER_BOX);
	~projectileclass();

	void SetDirVector(D3DXVECTOR3);
	D3DXVECTOR3 GetDirVector();
	bool checkDistance();
	
	void Move(float);
	bool CheckDestroy();

	int damage;
	int delay;
protected:


private:
	void InitializeProjectile();
	int distance;
	float speed;
	D3DXVECTOR3 dirVector;
};

#endif

