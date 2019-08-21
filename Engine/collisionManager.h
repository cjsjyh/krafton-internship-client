#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

class gameObject;
class gameManager;
class bossclass;
class playerclass;
class projectileclass;
class CameraClass;

using namespace std;

class collisionManager
{
public:
	collisionManager(gameManager*);
	~collisionManager();
	bool CollisionManager(vector<gameObject*>&, vector<gameObject*>&);
	bool CheckMovable(D3DXVECTOR3, D3DXVECTOR3);
	gameObject* InteractionManager(D3DXVECTOR3, int);

	int IsInsideMap(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3);
	CameraClass* m_Camera;

private:
	bool CheckMatch(gameObject*, gameObject*, int, int);
	bool CheckInOne(gameObject*, int, int);
	bool CheckAnyInOne(gameObject*, gameObject*, int, int);

	bool IsPlayer(gameObject*, gameObject*);
	bool IsBoss(gameObject*, gameObject*);

	bool CheckCollisionChannel(gameObject*, gameObject*);
	int CollisionHandler(gameObject*, gameObject*);
	bool InteractionCircularDetection(D3DXVECTOR3, int, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(D3DXVECTOR3, D3DXVECTOR3, gameObject*);
	bool MoveCollisionChannel(gameObject*);

	gameManager* GM;
	playerclass* player0;
};

#endif