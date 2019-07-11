#pragma once
#ifndef _PLAYERCLASS_H_
#define _PLYAERCLASS_H_

#include "hpobjects.h"

class playerclass : public hpobjects
{
public:
	playerclass(ModelClass*, int, D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0));
	~playerclass();

	void SetDirection(int);
	int GetDirection();

private:
	int direction;
};

#endif