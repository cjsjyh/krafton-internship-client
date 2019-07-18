#include "stdafx.h"

#include "gameObject.h"
#include "gameManager.h"
#include "playerclass.h"
#include "bossclass.h"
#include "projectileclass.h"

#include "collisionManager.h"

collisionManager::collisionManager(gameManager* _GM)
{
	GM = _GM;
}

bool collisionManager::CheckCollisionChannel(gameObject* obj1, gameObject* obj2)
{
	if (obj1->channel == gameObject::NO_COLLISION)
		return false;

	if (obj2->channel == gameObject::NO_COLLISION)
		return false;

	if (IsBullet(obj1) && IsBullet(obj2))
	{
		if (obj2->channel != obj1->channel)
			return true;
		return false;
	}

	if (obj2->channel != obj1->channel)
		return false;

	return true;
}

bool collisionManager::IsBullet(gameObject* obj)
{
	if (obj->GetName() == "playerbullet" || obj->GetName() == "bossbullet")
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

int collisionManager::CollisionHandler(gameObject* obj1, gameObject* obj2)
{
	if (IsBullet(obj1) || IsBullet(obj2))
	{
		
		if (IsBullet(obj1) && IsBullet(obj2))
		{
			GM->UnregisterObjectToRender(obj1);
			GM->UnregisterObjectToRender(obj2);
			return 1;
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

			boss->Hit(bullet->damage);

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
			playerclass* boss;
			projectileclass* bullet;
			if (obj1->GetName() == "player")
			{
				boss = (playerclass*)obj1;
				bullet = (projectileclass*)obj2;
				flag = 2;
			}
			else
			{
				boss = (playerclass*)obj2;
				bullet = (projectileclass*)obj1;
				flag = 1;
			}

			boss->Hit(bullet->damage);

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

			flag = false;
			//서로 타입이 같은 2물체
			if (srcType == destType)
			{
				if (srcType == gameObject::COLLIDER_BOX)
				{
					if (SimpleBoxCollision(GM->GetGameObject(i), GM->GetGameObject(j)))
						flag = CollisionHandler(GM->GetGameObject(i), GM->GetGameObject(j));
				}
			}


			//pop된게 없으므로 서로 충돌된 물체를 등록
			if (flag == 0)
			{
				item1.push_back(GM->GetGameObject(i));
				item2.push_back(GM->GetGameObject(j));
			}
			//i가 pop되었을 경우
			else if (flag == 1)
			{
				break;
			}
			else if (flag == 2)
			{
				i--;
				continue;
			}
			
		}

	}

	if (item1.size() == 0)
		return false;
	else
		return true;
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

bool collisionManager::ComplexCollision(gameObject* objSrc, gameObject* objDest)
{
	vector<D3DXVECTOR3> SrcCollPts = ComplexCollisionInitialize(objSrc);
	vector<D3DXVECTOR3> DestCollPts = ComplexCollisionInitialize(objDest);
	float distance = objSrc->sphere_collSize + objDest->sphere_collSize;

	vector<D3DXVECTOR3>::iterator iter1, iter2;

	for (iter1 = SrcCollPts.begin(); iter1 < SrcCollPts.end(); iter1++)
	{
		for (iter2 = DestCollPts.begin(); iter2 < DestCollPts.end(); iter2++)
		{
			float result = pow(iter1->x - iter2->x, 2) + pow(iter1->z - iter2->z, 2);
			if (result <= pow(distance, 2)) {
				cout << "HIT!!" << endl;
				return true;
			}
			else if (result >= pow(distance * 2, 2)) {
				return false;
			}
		}
	}
	return false;
}

bool collisionManager::SimpleComplexCollision(gameObject* objSimple, gameObject* objComplex)
{
	float collider_size = objComplex->sphere_collSize;
	vector<D3DXVECTOR3> objComplexPts = ComplexCollisionInitialize(objComplex);

	vector<D3DXVECTOR3>::iterator iter1;
	for (iter1 = objComplexPts.begin(); iter1 < objComplexPts.end(); iter1++)
	{
		if (SimpleDetection(objSimple, iter1, collider_size))
			return true;
	}

}

bool collisionManager::SimpleDetection(gameObject* simple, vector<D3DXVECTOR3>::iterator check, float colliderSize)
{
	D3DXVECTOR3 pos, len;
	pos = simple->GetPosition();
	len = simple->GetCollSize();
	if ((check->x - colliderSize <= pos.x + len.x && check->x + colliderSize >= pos.x - len.x) &&
		(check->z - colliderSize <= pos.z + len.z && check->z + colliderSize >= pos.z - len.z))
		return true;
	return false;
}

vector <D3DXVECTOR3> collisionManager::ComplexCollisionInitialize(gameObject* obj)
{
	vector <D3DXVECTOR3> points;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	//from object
	float minVertex;
	float collider_size = obj->sphere_collSize;


	pos = obj->GetPosition();
	size = obj->GetScale();
	minVertex = min(min(size.x, size.y), size.z);
	minVertex -= collider_size;

	for (int i = 1; i < (size.x / collider_size) * 2; i += 2) {
		for (int k = 1; k < (size.z / collider_size) * 2; k += 2) {
			D3DXVECTOR3 temp;
			float maxVertex;
			temp.x = pos.x - size.x + i * collider_size;
			temp.z = pos.z - size.z + k * collider_size;
			maxVertex = max(abs(temp.x), abs(temp.z));
			if (maxVertex < minVertex) {
				continue;
			}
			points.push_back(temp);
		}
	}

	return points;
}

