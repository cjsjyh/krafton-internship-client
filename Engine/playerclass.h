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

	void SetSpeed(float);

	void SetImage();
	void Move(int*);

private:
	int direction;
	float PLAYER_SPEED;
};

#endif