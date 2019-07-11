#include "hpobjects.h"

hpobjects::hpobjects(string name, ModelClass* model, int _hp, CollisionChannel coll, ColliderType type)
	:gameObject(name,model,coll,type)
{
	hp = _hp;
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