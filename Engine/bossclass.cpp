#include "bossclass.h"
bossclass::bossclass(ModelClass* model, int _hp, int _damage, ColliderType col)
	:gameObject("boss", model, col, HIT_BOSS)
{
	hp = _hp;
	damage = _hp;
	phase = 1;
}
bossclass::~bossclass()
{

}

void bossclass::Move()
{
	return;
}

bool bossclass::CheckDestroy()
{
	return false;
}

void bossclass::Hit(int _damage)
{
	hp -= _damage;
	cout << "boss hit! HP: " << to_string(hp) << endl;
	return;
}