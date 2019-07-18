#pragma once
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

class collisionManager;
class gameObject;
class bossclass;
class projectileclass;

class gameManager
{
public:

	struct CompareDist
	{
		bool operator()(gameObject* obj1, gameObject* obj2) {
			return obj1->w > obj2->w;
		}
	};

	gameManager(int);
	~gameManager();

	void RegisterObjectToRender(gameObject *item, int scene = 0);
	void UnregisterObjectToRender(gameObject *item, int scene = 0);
	void RemoveObjectToRender(gameObject* item, int scene = 0);
	int FindObjectIndex(gameObject *item, int scene = 0);
	
	void RegisterToBossPool(projectileclass* item);
	projectileclass* GetFromBossPool();
	void RegisterToPlayerPool(projectileclass* item);
	projectileclass* GetFromPlayerPool();

	int GetRenderObjectCount(int scene = 0);
	gameObject* GetGameObject(int index, int scene = 0);
	
	void CheckCollision(int scene = 0);

	void AlphaSort(D3DXVECTOR3, int scene = 0);

private:
	vector< vector<gameObject*> > renderObjects;
	vector <projectileclass*> BossbulletPool;
	vector <projectileclass*> PlayerbulletPool;

	collisionManager* m_CM;
	D3DXVECTOR3 camPos;

	
};

#endif