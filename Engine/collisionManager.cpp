#include "stdafx.h"

#include "gameObject.h"
#include "gameManager.h"
#include "playerclass.h"
#include "bossclass.h"
#include "projectileclass.h"
#include "cameraclass.h"

#include "socketInterface.h"

#include "collisionManager.h"

collisionManager::collisionManager(gameManager* _GM)
{
	GM = _GM;
	m_Camera = 0;
	player0 = 0;
}

collisionManager::~collisionManager()
{

}

bool collisionManager::CollisionManager(vector<gameObject*>& item1, vector<gameObject*>& item2)
{
	//check all objects if they collide and return true or false.
	//if there are objects colliding, add them to the vector
	int size = GM->GetRenderObjectCount();
	int flag;
	for (int i = size - 1; i > 0; i--) {
		gameObject::ColliderType srcType = GM->GetGameObject(i)->GetColliderType();
		for (int j = i - 1; j >= 0; j--) {
			gameObject::ColliderType destType = GM->GetGameObject(j)->GetColliderType();

			//collision을 체크할 필요가 없는 경우
			if (!CheckCollisionChannel(GM->GetGameObject(i), GM->GetGameObject(j)))
				continue;
			

			flag = -1;
			//서로 타입이 같은 2물체
			if (srcType == destType)
			{
				if (srcType == gameObject::COLLIDER_BOX)
				{
					if (SimpleBoxCollision(GM->GetGameObject(i), GM->GetGameObject(j)))
						flag = CollisionHandler(GM->GetGameObject(i), GM->GetGameObject(j));
				}
			}
			
			//충돌되지 않음
			if (flag == -1)
			{
				continue;
			}
			//pop된게 없으므로 서로 충돌된 물체를 등록
			else if (flag == 0)
			{
				item1.push_back(GM->GetGameObject(i));
				item2.push_back(GM->GetGameObject(j));
			}
			//i가 pop되었을 경우
			else if (flag == 1)
			{
				break;
			}
			//j가 pop되었을 경우
			else if (flag == 2)
			{
				i--;
				continue;
			}
			//i와 j둘다 pop
			else if (flag == 3)
			{
				i--;
				break;
			}
			

		}

	}
	if (item1.size() == 0)
		return false;
	else
		return true;
}

gameObject* collisionManager::InteractionManager(D3DXVECTOR3 point, int range)
{
	int size = GM->GetRenderObjectCount();
	for (int i = 0; i <size; i++) {
		if (GM->GetGameObject(i)->channel != gameObject::INTERACTION)
			continue;
		if (InteractionCircularDetection(point, range, GM->GetGameObject(i)))
			return GM->GetGameObject(i);
	}
	return NULL;
}

bool collisionManager::CheckMatch(gameObject* obj1, gameObject* obj2, int channel1, int channel2)
{
	if (obj1->channel == channel1)
		if (obj2->channel == channel2)
			return true;
	else if (obj1->channel == channel2)
		if (obj2->channel == channel1)
			return true;
	return false;
}

bool collisionManager::CheckAnyInOne(gameObject* obj1, gameObject* obj2, int channel1, int channel2)
{
	if (obj1->channel == channel1 || obj1->channel == channel2)
		return true;
	if (obj2->channel == channel1 || obj2->channel == channel2)
			return true;
	return false;
}

bool collisionManager::CheckInOne(gameObject* obj, int channel1, int channel2)
{
	if (obj->channel == channel1 || obj->channel == channel2)
		return true;
	return false;
}

bool collisionManager::IsPlayer(gameObject* obj1, gameObject* obj2)
{
	if (obj1->GetName() == "player" || obj2->GetName() == "player")
		return 1;
	return 0;
}

bool collisionManager::IsBoss(gameObject* obj1, gameObject* obj2)
{
	if (obj1->GetName() == "boss" || obj2->GetName() == "boss")
		return 1;
	return 0;
}

bool collisionManager::CheckCollisionChannel(gameObject* obj1, gameObject* obj2)
{
	if (obj1->channel == gameObject::NO_COLLISION)
		return false;

	else if (obj2->channel == gameObject::NO_COLLISION)
		return false;

	else if (CheckMatch(obj1, obj2, gameObject::PLAYER, gameObject::BOSS_BULLET))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::BOSS_BULLET, gameObject::PLAYER_BULLET))
		return false;

	else if (CheckMatch(obj1, obj2, gameObject::BOSS_BULLET_FOLLOW, gameObject::PLAYER_BULLET))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::BOSS_BULLET_FOLLOW, gameObject::PLAYER))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::BOSS, gameObject::PLAYER_BULLET))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::BOSS, gameObject::PLAYER))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::PLAYER, gameObject::INTERACTION))
		return true;

	else if (CheckMatch(obj1, obj2, gameObject::INTERACTION, gameObject::INTERACTION))
		return true;

	return false;
}

int collisionManager::CollisionHandler(gameObject* obj1, gameObject* obj2)
{
	if (CheckAnyInOne(obj1,obj2,gameObject::PLAYER_BULLET, gameObject::BOSS_BULLET))
	{
		string myBulletName = "player" + to_string(socketInterface::playerId) + "bullet";
		//BOTH ARE BULLETS
		if (CheckMatch(obj1, obj2, gameObject::BOSS_BULLET_FOLLOW, gameObject::PLAYER_BULLET))
		{
			if (obj1->tag == myBulletName || obj2->tag == myBulletName)
			{
				if (player0 == 0)
					player0 = (playerclass*)(GM->GetGameObjectByTag("player" + to_string(socketInterface::playerId)));
				if (socketInterface::curPlayerUltiGauge < player0->maxUltimateGauge)
				{
					socketInterface::curPlayerUltiGauge++;
				}
			}
			GM->UnregisterObjectToRender(obj1);
			GM->UnregisterObjectToRender(obj2);
			return 3;
		}

		if (IsBoss(obj1,obj2))
		{
			int flag;
			bossclass* boss;
			projectileclass* bullet;
			if(obj1->GetName() == "boss")
			{
				boss = (bossclass*)obj1;
				bullet = (projectileclass*)obj2;
				flag = 2;
			}
			else
			{
				boss = (bossclass*)obj2;
				bullet = (projectileclass*)obj1;
				flag = 1;
			}
			if (bullet->tag == myBulletName)
			{
				socketInterface::bossHitCount++;
				
				if (player0 == 0)
					player0 = (playerclass*)(GM->GetGameObjectByTag("player"+to_string(socketInterface::playerId)));
				if (socketInterface::curPlayerUltiGauge < player0->maxUltimateGauge)
				{
					socketInterface::curPlayerUltiGauge += 2;
				}
			}

			GM->UnregisterObjectToRender((gameObject*)bullet);
			if (bullet->GetName() == "playerbullet")
				GM->RegisterToPlayerPool((projectileclass*)bullet);
			else
				GM->RegisterToBossPool((projectileclass*)bullet);
			return flag;
		}
		if (IsPlayer(obj1,obj2))
		{
			int flag;
			playerclass* player;
			projectileclass* bullet;
			if (obj1->GetName() == "player")
			{
				player = (playerclass*)obj1;
				bullet = (projectileclass*)obj2;
				flag = 2;

			}
			else
			{
				player = (playerclass*)obj2;
				bullet = (projectileclass*)obj1;
				flag = 1;
			}
			if (player->tag == "player" + to_string(socketInterface::playerId))
			{
				m_Camera->SetCameraShake(10, 0.7);
				socketInterface::playerHitCount++;
			}
			
			//player->Hit(bullet->damage);

			GM->UnregisterObjectToRender((gameObject*)bullet);
			if (bullet->GetName() == "playerbullet")
				GM->RegisterToPlayerPool((projectileclass*)bullet);
			else
				GM->RegisterToBossPool((projectileclass*)bullet);
			return flag;
		}

	}
	return 0;
}

bool collisionManager::SimpleBoxCollision(gameObject* src, gameObject* dest)
{
	D3DXVECTOR3 pos, len;
	pos = src->GetPosition();
	len = src->GetCollSize();
	D3DXVECTOR3 pos2, len2;
	pos2 = dest->GetPosition();
	len2 = dest->GetCollSize();

	if ((pos2.x - len2.x <= pos.x + len.x && pos2.x + len2.x >= pos.x - len.x) &&
		(pos2.z - len2.z <= pos.z + len.z && pos2.z + len2.z >= pos.z - len.z))
		return true;
	return false;
}

bool collisionManager::SimpleBoxCollision(D3DXVECTOR3 pos, D3DXVECTOR3 len,gameObject* dest)
{
	D3DXVECTOR3 pos2, len2;
	pos2 = dest->GetPosition();
	len2 = dest->GetCollSize();
	if ((pos2.x - len2.x <= pos.x + len.x && pos2.x + len2.x >= pos.x - len.x) &&
		(pos2.z - len2.z <= pos.z + len.z && pos2.z + len2.z >= pos.z - len.z))
		return true;
	return false;
}

int collisionManager::IsInsideMap(D3DXVECTOR3 pos, D3DXVECTOR3 floorPos, D3DXVECTOR3 floorLen)
{
	//left wall
	if (floorPos.x - floorLen.x >= pos.x - 4)
		return 1;
	//right wall
	else if (floorPos.x + floorLen.x <= pos.x)
		return 3;
	//bottom wall
	else if (floorPos.z - floorLen.z >= pos.z)
		return 4;
	//top wall
	else if (floorPos.z + floorLen.z <= pos.z + 4)
		return 2;
	//inside map
	return 0;
}

bool collisionManager::CheckMovable(D3DXVECTOR3 pos, D3DXVECTOR3 len)
{
	for (int i = 0; i < GM->GetRenderObjectCount(); i++)
	{
		if (!MoveCollisionChannel(GM->GetGameObject(i)))
			continue;
		if (SimpleBoxCollision(pos, len, GM->GetGameObject(i)))
			return false;
	}
	return true;
}

bool collisionManager::MoveCollisionChannel(gameObject* obj)
{
	if (obj->channel == gameObject::NO_COLLISION)
		return false;
	if (obj->objType == gameObject::AUTOMOVE)
		return false;
	if (obj->GetName() == "player")
		return false;
	return true;
}

bool collisionManager::InteractionCircularDetection(D3DXVECTOR3 point, int range, gameObject* src)
{
	D3DXVECTOR3 pos, len;
	pos = src->GetPosition();
	len = src->GetCollSize();
	if (stdafx::GetDistance(point,pos) < range)
		return true;
	return false;
}


