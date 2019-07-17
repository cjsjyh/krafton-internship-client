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

	gameManager();
	~gameManager();

	void RegisterObjectToRender(gameObject *item);
	void UnregisterObjectToRender(gameObject *item);
	void RemoveObjectToRender(gameObject* item);
	int FindObjectIndex(gameObject *item);
	
	void RegisterToBossPool(projectileclass* item);
	projectileclass* GetFromBossPool();
	void RegisterToPlayerPool(projectileclass* item);
	projectileclass* GetFromPlayerPool();

	int GetRenderObjectCount();
	gameObject* GetGameObject(int index);
	
	void CheckCollision();

	void AlphaSort(D3DXVECTOR3);

private:
	vector <gameObject*> renderObjects;
	vector <projectileclass*> BossbulletPool;
	vector <projectileclass*> PlayerbulletPool;

	collisionManager* m_CM;
	D3DXVECTOR3 camPos;

	
};

#endif