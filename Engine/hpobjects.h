#pragma once
#ifndef _HPOBJECTS_H_
#define _HPOBJECTS_H_

class gameObject;
class D3DClass;
class ModelClass;

class hpobjects : public gameObject
{
public:
	hpobjects(string,  int, D3DClass*, CollisionChannel, ColliderType = COLLIDER_BOX);
	void HpObjectInitialize();
	void SetHp(int);
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

