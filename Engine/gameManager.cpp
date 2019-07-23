#include "stdafx.h"

#include "collisionManager.h"
#include "gameObject.h"
#include "bossclass.h"
#include "projectileclass.h"

#include <cstdlib>
#include <ctime>

#include "gameManager.h"

gameManager::gameManager(int sceneCount)
{
	for (int i = 0; i < sceneCount; i++)
	{
		vector<gameObject*> temp;
		renderObjects.push_back(temp);
	}

	for (int i = 0; i < ITEM_PHASE_COUNT; i++)
	{
		vector<Item> temp2;
		itemPool.push_back(temp2);
	}
	SetItemPool();
	m_CM = new collisionManager(this);
	srand(unsigned int(time(NULL)));
	floor = 0;
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

	return;
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

int gameManager::GetRenderObjectCount(int _scene)
{
	if(_scene != -1)
		return renderObjects[_scene].size();
	return renderObjects[scene].size();
}

gameObject* gameManager::GetGameObject(int index, int _scene)
{
	if(_scene != -1)
		return renderObjects[_scene][index];
	return renderObjects[scene][index];
}

void gameManager::CheckCollision()
{
	vector<gameObject*> coll1, coll2;
	m_CM->CollisionManager(coll1, coll2);
	return;
}

bool gameManager::CheckMovable(D3DXVECTOR3 pos, D3DXVECTOR3 len)
{
	return m_CM->CheckMovable(pos, len);
}


gameObject* gameManager::CheckInteraction(D3DXVECTOR3 point, int range)
{
	return m_CM->InteractionManager(point, range);
}

bool gameManager::CheckMapOut(D3DXVECTOR3 playerPos)
{
	D3DXMATRIX temp;
	//find floor and set
	if (floor == 0)
		for (auto iter = renderObjects[scene].begin(); iter != renderObjects[scene].end(); iter++)
			if ((*iter)->GetName() == "floor")
				floor = *iter;
	D3DXMatrixRotationY(&temp, -floor->GetRotation().y *0.0174532925f);
	D3DXVec3TransformCoord(&playerPos, &playerPos, &temp);
	return m_CM->IsInsideMap(playerPos, floor->GetPosition(), floor->GetScale());
}

void gameManager::SetItemPool()
{
	for (int i = 1; i < 6; i++)
	{
		Item temp;
		temp.name = to_string(i);
		temp.chosen = false;
		itemPool[0].push_back(temp);

		Item temp2;
		temp.name = to_string(i);
		temp.chosen = false;
		itemPool[1].push_back(temp2);
	}
	return;
}

string gameManager::ChooseItemFromPool(int phase)
{
	int index = rand() % itemPool[phase].size();
	return itemPool[phase][index].name;
}

void gameManager::SetItemUsed(int phase, string name)
{
	for (auto iter = itemPool[phase].begin(); iter != itemPool[phase].end(); iter++)
	{
		if ((*iter).name == name)
			(*iter).chosen = true;
	}
}
