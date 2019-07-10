#pragma once
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "gameObject.h"

#include "iostream"
#include <D3DX10math.h>
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

	

private:
	vector <gameObject*> gameobjects;
	
	bool SimpleComplexCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool ComplexCollision(gameObject*, gameObject*);

	bool SimpleDetection(gameObject*, vector<D3DXVECTOR3>::iterator, float);
	vector <D3DXVECTOR3> ComplexCollisionInitialize(gameObject*);
};

#endif