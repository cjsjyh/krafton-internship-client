#include "bossclass.h"
bossclass::bossclass(ModelClass* model, ColliderType col, int _hp, int _damage)
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