#pragma once
#ifndef _PLAYERCLASS_H_
#define _PLYAERCLASS_H_

#include "gameObject.h"

class playerclass : public gameObject
{
public:
	playerclass(ModelClass*, D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0));
	~playerclass();

	void SetDirection(int);
	int GetDirection();
private:
	int direction;
};

#endif