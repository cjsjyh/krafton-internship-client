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
	gameManager(int);
	~gameManager();

	void Frame();

	int scene;
	void RegisterObjectToRender(gameObject *item, int _scene=0);
	void UnregisterObjectToRender(gameObject *item, int _scene=0);
	void RemoveObjectToRender(gameObject* item, int _scene=0);
	int FindObjectIndex(gameObject *item, int _scene=0);
	int GetRenderObjectCount(int _scene = -1);
	gameObject* GetGameObject(int index, int _scene = -1);
	gameObject* GetGameObject(string name, int _scene = -1);

	void RegisterToBossPool(projectileclass* item);
	projectileclass* GetFromBossPool();
	void RegisterToPlayerPool(projectileclass* item);
	projectileclass* GetFromPlayerPool();
	
	void CheckCollision();
	bool CheckMovable(D3DXVECTOR3, D3DXVECTOR3);
	bool CheckMapOut(D3DXVECTOR3);
	void AutoMove();

	gameObject* CheckInteraction(D3DXVECTOR3, int);
	
	

	void AlphaSort(D3DXVECTOR3);

private:
	vector<vector<gameObject*>> renderObjects;
	
	vector<projectileclass*> BossbulletPool;
	vector<projectileclass*> PlayerbulletPool;

	collisionManager* m_CM;

	D3DXVECTOR3 camPos;

	gameObject* floor;
};

#endif