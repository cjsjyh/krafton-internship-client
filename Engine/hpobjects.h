#pragma once
#ifndef _HPOBJECTS_H_
#define _HPOBJECTS_H_

#include "gameObject.h"

class D3DClass;
class ModelClass;

class hpobjects : public gameObject
{
public:
	hpobjects(string,  int, D3DClass*, CollisionChannel, ColliderType = COLLIDER_BOX);
	~hpobjects();

	void SetHp(int);
	void ChangeHp(int);
	int GetHp();
	float GetHpPercent();

	void Hit(int);
	bool CheckDestroy();
	
	D3DXVECTOR3 normalizeVec3(D3DXVECTOR3);
protected:
	int maxHp;
	int curHp;
	vector<ModelClass*> model_list;
};

#endif

