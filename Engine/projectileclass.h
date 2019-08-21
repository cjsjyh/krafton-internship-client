#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameObject.h"
class ModelClass;
class D3DClass;

class projectileclass : public gameObject
{
public:
	projectileclass(string, D3DXVECTOR3, float _speed, int _damage, D3DClass*, gameObject::CollisionChannel, int _distance=100, int delay = 20, gameObject::ColliderType = COLLIDER_BOX);
	~projectileclass();

	void SetDirVector(D3DXVECTOR3);
	D3DXVECTOR3 GetDirVector();
	bool checkDistance();
	int GetFrame();

	void Move(float);
	bool CheckDestroy();
	void SetDistance(int);
	void SetDelay(int);
	void SetSpeed(float);

	int damage;
	int delay;
	int type;//0 = straight // 1 = follow player
protected:


private:
	void InitializeProjectile();
	int distance;
	float speed;
	D3DXVECTOR3 dirVector;
};

#endif

