#include "projectile.h"

projectile::projectile(string name, ModelClass* model, D3DXVECTOR3 pos, float _speed, int _distance, int _damage, CollisionChannel _channel, ColliderType coll)
	:gameObject(name,model,pos,_channel, coll)
{
	D3DXVECTOR3 position = GetPosition();
	objType = AUTOMOVE;
	SetScale(D3DXVECTOR3(0.5, 0.5, 0.5));
	distance = _distance;
	speed = _speed;
	damage = _damage;
}

bool projectile::checkDistance()
{
	if (distance-- < 0)
		return true;
	return false;
}

void projectile::SetDirVector(D3DXVECTOR3 dir)
{
	dirVector = dir * speed;
}

D3DXVECTOR3 projectile::GetDirVector()
{
	return dirVector;
}

void projectile::Move()
{
	AdjustPosition(dirVector);
}

bool projectile::CheckDestroy()
{
	return checkDistance();
}

