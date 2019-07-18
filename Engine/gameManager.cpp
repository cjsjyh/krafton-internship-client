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

void gameManager::RegisterObjectToRender(gameObject *item)
{
	renderObjects.push_back(item);
	return;
}

void gameManager::UnregisterObjectToRender(gameObject *item)
{
	int index = FindObjectIndex(item);
	gameObject* temp =  renderObjects[index];
	renderObjects.erase(renderObjects.begin() + index);
	return;
}

void gameManager::RemoveObjectToRender(gameObject* item)
{
	int index = FindObjectIndex(item);
	gameObject* temp = renderObjects[index];
	renderObjects.erase(renderObjects.begin() + index);
	delete temp;
	return;
}

void gameManager::AlphaSort(D3DXVECTOR3 _camPos)
{
	camPos = _camPos;
	for (auto iter = renderObjects.begin(); iter != renderObjects.end(); iter++)
		(*iter)->w = stdafx::GetDistance((*iter)->GetPosition(), _camPos);
	std::sort(renderObjects.begin(), renderObjects.end(), CompareDist());
}

void gameManager::RegisterToBossPool(projectileclass* item)
{
	BossbulletPool.push_back(item);
	return;
}

projectileclass* gameManager::GetFromBossPool()
{
	projectileclass* temp;
	if (BossbulletPool.size() == 0)
	{
		temp = NULL;
	}
	else
	{
		temp = BossbulletPool[BossbulletPool.size() - 1];
		BossbulletPool.pop_back();
	}
	return temp;
}

void gameManager::RegisterToPlayerPool(projectileclass* item)
{
	PlayerbulletPool.push_back(item);
	return;
}

projectileclass* gameManager::GetFromPlayerPool()
{
	projectileclass* temp;
	if (PlayerbulletPool.size() == 0)
	{
		temp = NULL;
	}
	else
	{
		temp = PlayerbulletPool[PlayerbulletPool.size() - 1];
		PlayerbulletPool.pop_back();
	}
	return temp;
}


int gameManager::FindObjectIndex(gameObject *item)
{
	vector<gameObject*>::iterator itr = find(renderObjects.begin(), renderObjects.end(), item);
	return distance(renderObjects.begin(), itr);
}

int gameManager::GetRenderObjectCount()
{
	return renderObjects.size();
}

gameObject* gameManager::GetGameObject(int index)
{
	return renderObjects[index];
}

void gameManager::CheckCollision()
{
	vector<gameObject*> coll1, coll2;
	m_CM->CollisionManager(coll1, coll2);
}
