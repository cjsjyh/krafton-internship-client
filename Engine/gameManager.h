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

	void RegisterObject(gameObject *item);
	void UnregisterObject(gameObject *item);
	int FindObjectIndex(gameObject *item);
	
	int GetObjectCount();
	gameObject* GetGameObject(int index);
	void CheckCollision();

	void AlphaSort(D3DXVECTOR3);

private:
	vector <gameObject*> gameobjects;
	collisionManager* m_CM;
	D3DXVECTOR3 camPos;

	
};

#endif