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



void gameManager::AlphaSort(D3DXVECTOR3 _camPos)
{
	camPos = _camPos;
	for (auto iter = gameobjects.begin(); iter != gameobjects.end(); iter++)
		(*iter)->w = stdafx::GetDistance((*iter)->GetPosition(), _camPos);
	std::sort(gameobjects.begin(), gameobjects.end(), CompareDist());
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
