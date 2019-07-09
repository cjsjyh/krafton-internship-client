#pragma once
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "gameObject.h"

#include "iostream"
#include <math.h>
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

	bool CollisionManager(vector<gameObject*>&, vector<gameObject*>&);

	struct coord {
		float x;
		float y;
		float z;
	};

private:
	vector <gameObject*> gameobjects;
	
	bool SimpleComplexCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool ComplexCollision(gameObject*, gameObject*);

	bool SimpleDetection(gameObject*, vector<gameManager::coord>::iterator, float);
	vector <coord> ComplexCollisionInitialize(gameObject*);
	

	float collider_size;
};

#endif