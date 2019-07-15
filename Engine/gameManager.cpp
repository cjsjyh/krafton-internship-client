#include "stdafx.h"

#include "collisionManager.h"
#include "gameObject.h"
#include "bossclass.h"
#include "projectileclass.h"

#include "gameManager.h"

gameManager::gameManager()
{
	m_CM = new collisionManager(this);
}

gameManager::~gameManager()
{
	delete m_CM;
}

void gameManager::RegisterObject(gameObject *item)
{
	gameobjects.push_back(item);
	cout << "manager size: " << gameobjects.size() << endl;
	return;
}

void gameManager::UnregisterObject(gameObject *item)
{
	int index = FindObjectIndex(item);
	gameObject* temp =  gameobjects[index];
	gameobjects.erase(gameobjects.begin() + index);
	delete temp;
	return;
}

int gameManager::FindObjectIndex(gameObject *item)
{
	vector<gameObject*>::iterator itr = find(gameobjects.begin(), gameobjects.end(), item);
	return distance(gameobjects.begin(), itr);
}

int gameManager::GetObjectCount()
{
	return gameobjects.size();
}

gameObject* gameManager::GetGameObject(int index)
{
	return gameobjects[index];
}

void gameManager::CheckCollision()
{
	vector<gameObject*> coll1, coll2;
	m_CM->CollisionManager(coll1, coll2);
}
