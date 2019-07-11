#include "bossclass.h"
bossclass::bossclass(ModelClass* model, int _hp, int _damage, ColliderType col)
	:hpobjects("boss", model, _hp, HIT_BOSS, col)
{
	hp = _hp;
	damage = _hp;
	phase = 1;
}
bossclass::~bossclass()
{

}