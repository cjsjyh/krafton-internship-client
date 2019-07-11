#pragma once
#ifndef _BOSSCLASS_H_
#define _BOSSCLASS_H_

#include "gameObject.h"

class bossclass : public gameObject
{
public:
	bossclass();
	~bossclass();

	void Move();
	bool CheckDestory();
private:
	int hp;
	int phase;
	int damage;
};

#endif

