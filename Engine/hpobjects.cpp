#include "hpobjects.h"

hpobjects::hpobjects(string name, int _hp, D3DClass* _device, CollisionChannel coll, ColliderType type)
	:gameObject(name,coll,type)
{
	hp = _hp;
	device = _device;
	HpObjectInitialize();
}

void hpobjects::HpObjectInitialize()
{
	string tName = "../Engine/data/" + GetName() + ".dds";
	m_model->Initialize(device->GetDevice(), "../Engine/data/plane.txt", StringToWchar(tName));
}

void hpobjects::Hit(int _damage)
{
	hp -= _damage;
	CheckDestroy();
	cout << GetName() << " Hit! HP: " << to_string(hp) << endl;
}

bool hpobjects::CheckDestroy()
{
	if (hp <= 0)
	{
		cout << GetName() << " dead" << endl;
		return true;
	}
	return false;
}

D3DXVECTOR3 hpobjects::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, 0, vec.z / square);
}