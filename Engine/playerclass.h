#pragma once
#ifndef _PLAYERCLASS_H_
#define _PLYAERCLASS_H_

#include "hpobjects.h"
#include "projectile.h"

class playerclass : public hpobjects
{
public:
	playerclass(int, D3DClass* ,D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
	~playerclass();

	projectile* Fire(D3DXVECTOR3);
	
	void SetDirection(char*);
	int GetDirection();

	void SetSpeed(float);

	void SetImage();
	void Move(char*);

private:
	int direction;
	float PLAYER_SPEED;
};

#endif