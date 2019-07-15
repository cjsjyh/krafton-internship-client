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
	gameManager();
	~gameManager();

	void RegisterObject(gameObject *item);
	void UnregisterObject(gameObject *item);
	int FindObjectIndex(gameObject *item);
	
	int GetObjectCount();
	gameObject* GetGameObject(int index);
	void CheckCollision();
	

private:
	vector <gameObject*> gameobjects;
	collisionManager* m_CM;
};

#endif