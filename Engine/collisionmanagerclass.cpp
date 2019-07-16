#include "collisionmanagerclass.h"


#include "gameManager.h"
#include "bossclass.h"
#include "projectileclass.h"

collisionmanagerclass::collisionmanagerclass()
{
}

bool collisionmanagerclass::CheckCollisionChannel(gameObject* obj1, gameObject* obj2)
{
	if (obj1->channel == gameObject::NO_COLLISION)
		return false;

	if (obj2->channel == gameObject::NO_COLLISION)
		return false;

	if (obj1->GetName() == "bullet" && obj2->GetName() == "bullet")
		return true;

	if (obj2->channel != obj1->channel)
		return false;

	return true;
}

int collisionmanagerclass::CollisionHandler(gameManager* GM, gameObject* obj1, gameObject* obj2)
{
	if (obj1->GetName() == "bullet" || obj2->GetName() == "bullet")
	{
		//both are bullet
		if (obj1->GetName() == obj2->GetName())
		{
			//only if bullets are fired from player and the boss
			if (obj1->channel != obj2->channel)
			{
				GM->UnregisterObject(obj1);
				GM->UnregisterObject(obj2);
				return 1;
			}
		}

		if (obj1->GetName() == "boss")
		{
			bossclass* boss = (bossclass*)obj1;
			projectileclass* bullet = (projectileclass*)obj2;

			boss->Hit(bullet->damage);
			GM->UnregisterObject(obj2);
			return 2;
		}
		else if (obj2->GetName() == "boss")
		{
			bossclass* boss = (bossclass*)obj2;
			projectileclass* bullet = (projectileclass*)obj1;

			boss->Hit(bullet->damage);
			GM->UnregisterObject(obj1);
			return 1;
		}

		if (obj1->GetName() == "player")
		{
			playerclass* player = (playerclass*)obj1;
			projectileclass* bullet = (projectileclass*)obj2;

			player->Hit(bullet->damage);
			GM->UnregisterObject(obj2);
			return 2;
		}
		else if (obj2->GetName() == "player")
		{
			playerclass* player = (playerclass*)obj2;
			projectileclass* bullet = (projectileclass*)obj1;

			player->Hit(bullet->damage);
			GM->UnregisterObject(obj1);
			return 1;
		}
	}
	return 0;
}

bool collisionmanagerclass::CollisionManager(gameManager* GM, vector<gameObject*>& item1, vector<gameObject*>& item2)
{
	//check all objects if they collide and return true or false.
	//if there are objects colliding, add them to the vector
	int size = GM->GetObjectCount();
	int flag;
	for (int i = size - 1; i > 0; i--) {
		while (i >= size)
			i--;

		gameObject::ColliderType srcType = GM->GetGameObject(i)->GetColliderType();
		for (int j = i - 1; j >= 0; j--) {
			gameObject::ColliderType destType = GM->GetGameObject(j)->GetColliderType();

			//collision�� üũ�� �ʿ䰡 ���� ���
			if (!CheckCollisionChannel(GM->GetGameObject(i), GM->GetGameObject(j)))
				continue;

			flag = false;
			//���� Ÿ���� ���� 2��ü
			if (srcType == destType)
			{
				if (srcType == gameObject::COLLIDER_BOX)
				{
					if (SimpleBoxCollision(GM->GetGameObject(i), GM->GetGameObject(j)))
					{
						flag = CollisionHandler(GM, GM->GetGameObject(i), GM->GetGameObject(j));
					}
				}
			}

			//pop�Ȱ� �����Ƿ� ���� �浹�� ��ü�� ���
			if (flag == 0)
			{
				item1.push_back(GM->GetGameObject(i));
				item2.push_back(GM->GetGameObject(j));
			}
			//i�� pop�Ǿ��� ���
			else if (flag == 1)
				break;
			else if (flag == 2)
				continue;
		}

	}

	if (item1.size() == 0)
		return false;
	else
		return true;
}

bool collisionmanagerclass::SimpleBoxCollision(gameObject* src, gameObject* dest)
{
	D3DXVECTOR3 pos, len;
	pos = src->GetPosition();
	len = src->GetSize();
	D3DXVECTOR3 pos2, len2;
	pos2 = dest->GetPosition();
	len2 = dest->GetSize();

	if ((pos2.x - len2.x <= pos.x + len.x && pos2.x + len2.x >= pos.x - len.x) &&
		(pos2.z - len2.z <= pos.z + len.z && pos2.z + len2.z >= pos.z - len.z))
		return true;
	return false;
}

bool collisionmanagerclass::ComplexCollision(gameObject* objSrc, gameObject* objDest)
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

bool collisionmanagerclass::SimpleComplexCollision(gameObject* objSimple, gameObject* objComplex)
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

bool collisionmanagerclass::SimpleDetection(gameObject* simple, vector<D3DXVECTOR3>::iterator check, float colliderSize)
{
	D3DXVECTOR3 pos, len;
	pos = simple->GetPosition();
	len = simple->GetSize();
	if ((check->x - colliderSize <= pos.x + len.x && check->x + colliderSize >= pos.x - len.x) &&
		(check->z - colliderSize <= pos.z + len.z && check->z + colliderSize >= pos.z - len.z))
		return true;
	return false;
}

vector <D3DXVECTOR3> collisionmanagerclass::ComplexCollisionInitialize(gameObject* obj)
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