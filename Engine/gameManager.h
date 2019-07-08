#pragma once
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "gameObject.h"

#include "iostream"
#include <math.h>
#include <vector>
using namespace std;

#define COLLIDER_SIZE 0.5

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

	bool CollisionManager(vector<gameObject*>&, vector<gameObject*>&);

	struct coord {
		float x;
		float y;
		float z;
	};

private:
	

	bool DetectCollision(vector<gameManager::coord>, vector<gameManager::coord>);
	vector <coord> GetColliderCenter(gameObject*);
	vector <gameObject*> gameobjects;


};

#endif