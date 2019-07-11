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
	projectile(string, ModelClass*, D3DXVECTOR3, float speed, int distance, CollisionChannel = HIT_PLAYER, ColliderType = COLLIDER_BOX);
	~projectile();

	void SetDirVector(D3DXVECTOR3);
	D3DXVECTOR3 GetDirVector();

	bool checkDistance();
	void Move();
	bool CheckDestroy();

protected:


private:
	int distance;
	float speed;
	D3DXVECTOR3 dirVector;
};

#endif

