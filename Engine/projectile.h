#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameObject.h"
#include "modelclass.h"

#include <d3dx10math.h>

#include <string>
#include "iostream"

using namespace std;

class projectile : public gameObject
{
public:
	projectile(string, ModelClass*, ColliderType, D3DXVECTOR3, float, gameObject::CollisionChannel=HIT_PLAYER);
	~projectile();

	bool checkDistance();
	void Move();

protected:


private:
	int distance;
	float speed;
	D3DXVECTOR3 dirVector;

	void GetDirVector();
};

#endif

