#include "bossclass.h"
bossclass::bossclass(ModelClass* model, ModelClass* bullet, playerclass* _player, int _hp, int _damage, ColliderType col)
	:hpobjects("boss", model, _hp, HIT_BOSS, col)
{
	hp = _hp;
	damage = _hp;
	phase = 1;
	player = _player;
	bullet_model = bullet;
}
bossclass::~bossclass()
{

}

projectile* bossclass::Fire()
{
	projectile* temp = new projectile("bullet", bullet_model, GetPosition(), 1, 100, 3, gameObject::HIT_PLAYER);
	temp->SetDirVector(normalizeVec3(player->GetPosition() - GetPosition()));
	return temp;
}

D3DXVECTOR3 bossclass::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, 0, vec.z / square);
}