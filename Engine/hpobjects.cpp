#include "stdafx.h"

#include "gameObject.h"
#include "d3dclass.h"
#include "modelclass.h"

#include "hpobjects.h"

hpobjects::hpobjects(string name, int _hp, D3DClass* _device, CollisionChannel coll, ColliderType type)
	:gameObject(name,coll,type)
{
	curHp = _hp;
	device = _device;
	maxHp = 0;
	//HpObjectInitialize();
}

hpobjects::~hpobjects()
{

}

void hpobjects::SetHp(int hp)
{
	maxHp = curHp = hp;
}

int hpobjects::GetHp()
{
	return curHp;
}

float hpobjects::GetHpPercent()
{
	return (float)curHp / maxHp;
}

void hpobjects::ChangeHp(int hp)
{
	curHp = hp;
}

void hpobjects::Hit(int _damage)
{
	curHp -= _damage;
	CheckDestroy();
	cout << GetName() << " Hit! HP: " << to_string(curHp) << endl;
}

bool hpobjects::CheckDestroy()
{
	if (curHp <= 0)
	{
		cout << GetName() << " dead" << endl;
		return true;
	}
	return false;
}

D3DXVECTOR3 hpobjects::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.y*vec.y + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, vec.y / square, vec.z / square);
}