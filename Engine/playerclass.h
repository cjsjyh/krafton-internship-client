#pragma once
#ifndef _PLAYERCLASS_H_
#define _PLYAERCLASS_H_

#include "hpobjects.h"

class projectileclass;
class D3DClass;
class ModelClass;
class gameObject;

#define PLAYER_IMAGE_NUM 8

class playerclass : public hpobjects
{
public:
	playerclass(int, D3DClass* ,D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
	~playerclass();

	void InitializeModels();
	projectileclass* Fire(D3DXVECTOR3);
	
	void SetDirection(int*);
	int GetDirection();
	D3DXVECTOR3 GetDirectionVector(int);

	void SetSpeed(float);

	void SetImage();
	void Move(int*, int);

	//player movement parameters
	float PLAYER_SPEED;
	int PLAYER_DASH_SPEED;
	int PLAYER_DASH_FRAME;
	int PLAYER_DASH_PAUSE_FRAME;

	//player bullet parameters
	int PLAYER_BULLET_DAMAGE;
	float PLAYER_BULLET_SPEED;
	int PLAYER_BULLET_DISTANCE;
	int PLAYER_BULLET_DELAY;
	

private:
	int direction;
	int dashFrame, dashPauseFrame;
	int dashDir;
};

#endif