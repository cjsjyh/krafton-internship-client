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

	typedef struct BossPattern {
		PatternType type;
		int dirCount;
		int Angle;
		int repeat;
	};

	bossclass(int _hp, int _damage, D3DClass*, playerclass*,  ColliderType col = COLLIDER_BOX);
	~bossclass();

	vector<projectileclass*> Frame(int);
	void SetGameManager(gameManager*);

	float BOSS_PHASE2_HP;
	float BOSS_PHASE3_HP;
	D3DXVECTOR3 BOSS_SIZE[3];
private:
	int phase;
	int damage;

	playerclass* player;
	ModelClass* bullet_model;

	void Initialize();
	void InitializeModels();
	void CheckHp();

	void Fire();
	void FireDirections(vector<D3DXVECTOR3>, int delay =0);
	void SetBullet(projectileclass*, D3DXVECTOR3);

	void PushQueue(projectileclass*, int);
	void CheckQueue();
	void PopQueue(vector<projectileclass*>&);

	void SetBossPhasePattern();
	void ActivatePattern(BossPattern);
	vector<vector<BossPattern>> bosspattern;
	vector<BulletQueue> bossBullets;
	gameManager* GM;
};

#endif

