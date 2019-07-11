#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float _speed, int _distance, CollisionChannel _channel)
	:gameObject(name,model,coll,pos,_channel)
{
	D3DXVECTOR3 position = GetPosition();
	speed = _speed;
	objType = AUTOMOVE;
	SetScale(D3DXVECTOR3(0.5, 0.5, 0.5));
	distance = _distance;
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

