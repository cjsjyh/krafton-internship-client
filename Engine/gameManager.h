#pragma once
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "gameObject.h"

#include "iostream"
#include <vector>
using namespace std;

class gameManager
{
public:
	gameManager();
	~gameManager();

	void RegisterObject(gameObject *item);
	void UnregisterObject(gameObject *item);
	int FindObjectIndex(gameObject *item);
	
	int GetObjectCount();
	gameObject* GetGameObject(int index);

private:
	//bool DetectCollision(gameObject, gameObject);
	
	vector <gameObject*> gameobjects;
};

#endif