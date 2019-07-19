#include "stdafx.h"

#include "collisionManager.h"
#include "gameObject.h"
#include "bossclass.h"
#include "projectileclass.h"

#include "gameManager.h"

gameManager::gameManager(int sceneCount)
{
	for (int i = 0; i < sceneCount; i++)
	{
		vector<gameObject*> temp, temp2;
		renderObjects.push_back(temp);
	}
	m_CM = new collisionManager(this);
}

gameManager::~gameManager()
{
	delete m_CM;
}


void gameManager::RegisterObjectToRender(gameObject *item, int _scene)
{
	renderObjects[_scene].push_back(item);
	return;
}

void gameManager::UnregisterObjectToRender(gameObject *item, int _scene)
{
	int index = FindObjectIndex(item, _scene);
	gameObject* temp =  renderObjects[_scene][index];
	renderObjects[_scene].erase(renderObjects[_scene].begin() + index);
	return;
}

void gameManager::RemoveObjectToRender(gameObject* item, int _scene)
{
	int index = FindObjectIndex(item, _scene);
	gameObject* temp = renderObjects[_scene][index];
	renderObjects[_scene].erase(renderObjects[_scene].begin() + index);
	delete temp;
	return;
}

void gameManager::AlphaSort(D3DXVECTOR3 _camPos)
{
	camPos = _camPos;
	for (auto iter = renderObjects[scene].begin(); iter != renderObjects[scene].end(); iter++)
	{
		if ((*iter)->GetName() == "floor")
			(*iter)->w = 9999999;
		else
			(*iter)->w = stdafx::GetDistance((*iter)->GetPosition(), _camPos);
	}
	std::sort(renderObjects[scene].begin(), renderObjects[scene].end(), CompareDist());
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


int gameManager::FindObjectIndex(gameObject *item, int _scene)
{
	vector<gameObject*>::iterator itr = find(renderObjects[_scene].begin(), renderObjects[_scene].end(), item);
	return distance(renderObjects[_scene].begin(), itr);
}

int gameManager::GetRenderObjectCount()
{
	return renderObjects[scene].size();
}

gameObject* gameManager::GetGameObject(int index)
{
	return renderObjects[scene][index];
}

void gameManager::CheckCollision()
{
	vector<gameObject*> coll1, coll2;
	m_CM->CollisionManager(coll1, coll2);
	return;
}

gameObject* gameManager::CheckInteraction(D3DXVECTOR3 point)
{
	return m_CM->InteractionManager(point);
}