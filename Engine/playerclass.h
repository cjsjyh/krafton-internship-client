#pragma once
#ifndef _PLAYERCLASS_H_
#define _PLYAERCLASS_H_

#include "hpobjects.h"

class projectileclass;
class D3DClass;
class ModelClass;
class gameObject;
class gameManager;
class itemmanagerclass;

#define PLAYER_IMAGE_NUM 8

class playerclass : public hpobjects
{
public:
	playerclass(int, D3DClass* ,D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
	~playerclass();
	
	void InitializeModels();
	
	void Frame(int*, int);

	void SetManager(gameManager*, itemmanagerclass*);
	void SetDirection(int*);
	void SetDirection(int);
	int GetDirection();
	void SetImage();
	
	D3DXVECTOR3 GetDirectionVector(int);
	projectileclass* Fire(D3DXVECTOR3);
	void SetBullet(projectileclass*, D3DXVECTOR3);
	int ObjectInteraction(int*);

	bool Dash(int*, int);
	void SetSpeed(float);

	void SavePlayerPos(int scene);
	D3DXVECTOR3 GetSavedPlayerPos(int scene);

	//player movement parameters
	float PLAYER_SPEED;
	int PLAYER_DASH_SPEED;
	int PLAYER_DASH_FRAME;
	int PLAYER_DASH_PAUSE_FRAME;
	float PLAYER_INTERACTION_RANGE;

	//player bullet parameters
	int PLAYER_BULLET_DAMAGE;
	float PLAYER_BULLET_SPEED;
	int PLAYER_BULLET_DISTANCE;
	int PLAYER_BULLET_DELAY;

private:
	int direction;
	int dashFrame, dashPauseFrame;
	int dashDir;
	vector<D3DXVECTOR3> playerPosSave;
	gameManager* GM;
	itemmanagerclass* IM;
};

#endif