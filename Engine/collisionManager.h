#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

class gameObject;
class gameManager;
class bossclass;
class playerclass;
class projectileclass;

using namespace std;

class collisionManager
{
public:
	collisionManager(gameManager*);
	bool CollisionManager(vector<gameObject*>&, vector<gameObject*>&);

private:
	int CollisionHandler(gameObject*, gameObject*);
	bool CheckCollisionChannel(gameObject*, gameObject*);

	bool SimpleComplexCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool ComplexCollision(gameObject*, gameObject*);

	bool SimpleDetection(gameObject*, vector<D3DXVECTOR3>::iterator, float);
	vector<D3DXVECTOR3> ComplexCollisionInitialize(gameObject*);

	gameManager* GM;
};

#endif