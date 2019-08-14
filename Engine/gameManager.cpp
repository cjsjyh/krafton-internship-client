#include "stdafx.h"

#include "collisionManager.h"
#include "gameObject.h"
#include "bossclass.h"
#include "projectileclass.h"
#include "cameraclass.h"

#include <stdlib.h>
#include <ctime>

#include "gameManager.h"

struct CompareDist
{
	bool operator()(gameObject* obj1, gameObject* obj2) {
		return obj1->w > obj2->w;
	}
};

gameManager::gameManager(int sceneCount)
{
	for (int i = 0; i < sceneCount; i++)
	{
		vector<gameObject*> temp;
		renderObjects.push_back(temp);
	}
	
	m_CM = new collisionManager(this);
	
	floor = 0;
	srand((unsigned int)time(NULL));
}

gameManager::~gameManager()
{
	delete m_CM;
}

void gameManager::Frame()
{
	AutoMove();
	CheckCollision();
}

void gameManager::RegisterObjectToRender(gameObject *item, int _scene)
{
	if (item->GetName() == "player")
		players.push_back(item);
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
	if (index > renderObjects[_scene].size())
		return;

	gameObject* temp = renderObjects[_scene][index];
	renderObjects[_scene].erase(renderObjects[_scene].begin() + index);
	delete temp;
	temp = 0;
	return;
}

void gameManager::RemoveObjectToRender(string item, int _scene)
{
	int index = FindObjectIndex(item, _scene);
	if (index > renderObjects[_scene].size())
		return;

	gameObject* temp = renderObjects[_scene][index];
	renderObjects[_scene].erase(renderObjects[_scene].begin() + index);
	delete temp;
	temp = 0;
	return;
}

void gameManager::AlphaSort()
{
	camPos = m_Camera->GetPosition();
	for (auto iter = renderObjects[scene].begin(); iter != renderObjects[scene].end(); iter++)
	{
		if ((*iter)->GetName() == "floor")
			(*iter)->w = 9999999;
		else
		{
			D3DXVECTOR3 v1 = D3DXVECTOR3(0, (*iter)->GetPosition().y, (*iter)->GetPosition().z);
			D3DXVECTOR3 v2 = D3DXVECTOR3(0, camPos.y, camPos.z);
			(*iter)->w = stdafx::GetDistance(v1, v2);
		}
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

int gameManager::FindObjectIndex(string item, int _scene)
{
	for (auto iter = renderObjects[_scene].begin(); iter != renderObjects[_scene].end(); iter++)
	{
		if((*iter)->GetName() == item)
			return distance(renderObjects[_scene].begin(), iter);
	}
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

gameObject* gameManager::GetGameObject(string name, int _scene)
{
	int curScene;
	if (_scene != -1)
		curScene = _scene;
	else
		curScene = scene;

	for (auto iter = renderObjects[curScene].begin(); iter != renderObjects[curScene].end(); iter++)
	{
		if ((*iter)->GetName() == name)
			return *iter;
	}
	return NULL;
}

gameObject* gameManager::GetGameObjectByTag(string tag, int _scene)
{
	int curScene;
	if (_scene != -1)
		curScene = _scene;
	else
		curScene = scene;

	for (auto iter = renderObjects[curScene].begin(); iter != renderObjects[curScene].end(); iter++)
	{
		if ((*iter)->tag == tag)
			return *iter;
	}
	return NULL;
}

void gameManager::CheckCollision()
{
	vector<gameObject*> coll1, coll2;
	m_CM->CollisionManager(coll1, coll2);
	if (!m_CM->m_Camera)
		m_CM->m_Camera = m_Camera;

	if (coll1.size() > 0)
	{
		if (CheckCollisionObjects("playerbullet", "boss", coll1, coll2))
			printf("player hit boss!\n");
	}

	return;
}

bool gameManager::CheckMovable(D3DXVECTOR3 pos, D3DXVECTOR3 len)
{
	return m_CM->CheckMovable(pos, len);
}

void gameManager::RemoveAllBullets()
{
	vector<int> bulletIndex;
	for (int i = renderObjects[0].size()-1; i >= 0; i--)
	{
		gameObject* tempObj = renderObjects[0][i];
		if (tempObj->GetName() == "bossbullet")
		{
			//bulletIndex.push_back(distance(iter, renderObjects[0].begin()));
			UnregisterObjectToRender(tempObj, scene);
			RegisterToBossPool((projectileclass*)(tempObj));
		}
	}
}

void gameManager::AutoMove()
{
	//Move AUTOMOVE objects
	int size = GetRenderObjectCount();
	for (int i = size - 1; i >= 0; i--)
	{
		gameObject* temp = GetGameObject(i);
		if (temp->objType == gameObject::AUTOMOVE)
		{
			projectileclass* bullet = (projectileclass*)temp;

			//Move slow
			if (bullet->delay > 0)
			{
				bullet->delay--;
				bullet->Move(0.1);
			}
			//Move at normal speed
			else
			{
				switch (bullet->type)
				{
				//follow player
				case 1:
					cout << "Follow Player!!" << endl;
					int targetIndex = rand() % players.size();
					bullet->SetDirVector(stdafx::normalizeVec3(players[0]->GetPosition() - bullet->GetPosition()));
					break;
				}

				bullet->Move(1);
				if (temp->CheckDestroy())
				{
					if (temp->GetName() == "bossbullet")
					{
						UnregisterObjectToRender(temp, scene);
						RegisterToBossPool((projectileclass*)temp);
					}
					else if (temp->GetName() == "playerbullet")
					{
						UnregisterObjectToRender(temp, scene);
						RegisterToPlayerPool((projectileclass*)temp);
					}
					else
					{
						RemoveObjectToRender(temp, scene);
					}
				}
			}
		}
	}
	return;
}

gameObject* gameManager::CheckInteraction(D3DXVECTOR3 point, int range)
{
	return m_CM->InteractionManager(point, range);
}

bool gameManager::CheckMapOut(D3DXVECTOR3 playerPos)
{
	D3DXMATRIX temp;
	//find floor and set
	if (!floor)
	{
		for (auto iter = renderObjects[scene].begin(); iter != renderObjects[scene].end(); iter++)
			if ((*iter)->GetName() == "floor")
				floor = *iter;
	}
	D3DXMatrixRotationY(&temp, -floor->GetRotation().y *0.0174532925f);
	D3DXVec3TransformCoord(&playerPos, &playerPos, &temp);
	return m_CM->IsInsideMap(playerPos, floor->GetPosition(), floor->GetScale());
}

bool gameManager::CheckCollisionObjects(string n1, string n2, vector<gameObject*> v1, vector<gameObject*> v2)
{
	for (auto iter1 = v1.begin(); iter1 != v1.end(); iter1++)
	{
		if ((*iter1)->GetName() == n1)
		{
			for (auto iter2 = v2.begin(); iter2 != v2.end(); iter2++)
			{
				if ((*iter2)->GetName() == n2)
					return true;
			}
		}
		else if ((*iter1)->GetName() == n2)
		{
			for (auto iter2 = v2.begin(); iter2 != v2.end(); iter2++)
			{
				if ((*iter2)->GetName() == n1)
					return true;
			}
		}
	}
	return false;
}