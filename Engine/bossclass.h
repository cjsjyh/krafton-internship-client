#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "hpobjects.h"

#define BOSS_PHASE_NUM 3

class projectileclass;
class playerclass;
class D3DClass;
class gameObject;
class gameManager;

class bossclass : public hpobjects
{
public:
	enum PatternType
	{
		FIRE_AT_PLAYER,
		FIRE_IN_FAN,
		FIRE_ALL_DIR,
	};

	typedef struct BulletQueue{
		projectileclass* bullet;
		int frame;
	}BulletQueue;


	bossclass(int _hp, int _damage, D3DClass*, vector<playerclass*>,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	void Frame(int);
	void SetGameManager(gameManager*);
	void ClearBullet();

	D3DXVECTOR3 BOSS_SIZE[3];
	vector<vector<BossPatternFile>> patternFile;
private:
	int phase;
	int damage;

	vector<playerclass*> player;
	ModelClass* bullet_model;

	void Initialize();
	void InitializeModels();
	void CheckHp();

	void FireDirections(vector<D3DXVECTOR3>, int fireDelay =0, int distance=100, int slowFrame=20, float bulletSpeed=1, int bulletType =0);
	void SetBullet(projectileclass*, D3DXVECTOR3, int distance = 100, int slowFrame=20, float bulletSpeed=1, int bulletType = 0);

	void PushQueue(projectileclass*, int);
	void PopQueue(vector<projectileclass*>&);

	void ActivatePattern(BossPatternFile, int);
	vector<vector<BossPatternFile>> bossPatternPool;
	vector<BulletQueue> bossBullets;
	gameManager* GM;
	ModelClass* BulletModelBasic;
	ModelClass* BulletModelFollow;
};

#endif

