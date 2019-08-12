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


	bossclass(int _hp, int _damage, D3DClass*, playerclass*,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	void Frame(int);
	void SetGameManager(gameManager*);

	D3DXVECTOR3 BOSS_SIZE[3];
	vector<vector<BossPatternFile>> patternFile;
private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;

	void Initialize();
	void InitializeModels();
	void CheckHp();

	void Fire(int delay = 0);
	void FireDirections(vector<D3DXVECTOR3>, int delay =0, int distance=100);
	void SetBullet(projectileclass*, D3DXVECTOR3, int distance = 100);

	void PushQueue(projectileclass*, int);
	void PopQueue(vector<projectileclass*>&);

	//void SetBossPhasePattern();
	void ActivatePattern(BossPatternFile);
	vector<vector<BossPatternFile>> bossPatternPool;
	vector<BulletQueue> bossBullets;
	gameManager* GM;
};

#endif

