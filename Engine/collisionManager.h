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
	gameObject* InteractionManager(D3DXVECTOR3);
private:
	bool CheckMatch(gameObject*, gameObject*, int, int);
	bool CheckInOne(gameObject*, int, int);
	bool CheckAnyInOne(gameObject*, gameObject*, int, int);

	bool IsPlayer(gameObject*, gameObject*);
	bool IsBoss(gameObject*, gameObject*);

	bool CheckCollisionChannel(gameObject*, gameObject*);
	int CollisionHandler(gameObject*, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(D3DXVECTOR3, gameObject*);

	gameManager* GM;
};

#endif