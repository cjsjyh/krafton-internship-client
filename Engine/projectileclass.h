#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameObject.h"
#include "modelclass.h"

#include <d3dx10math.h>

#include <string>
#include "iostream"

using namespace std;

class projectileclass : public gameObject
{
public:
	projectileclass(string, D3DXVECTOR3, float _speed, int _distance, int _damage, D3DClass*, CollisionChannel = HIT_PLAYER, ColliderType = COLLIDER_BOX);
	~projectileclass();

	void SetDirVector(D3DXVECTOR3);
	D3DXVECTOR3 GetDirVector();
	bool checkDistance();
	
	void Move();
	bool CheckDestroy();

	int damage;
protected:


private:
	void InitializeProjectile();

	int distance;
	float speed;
	D3DXVECTOR3 dirVector;
};

#endif

