#include "projectile.h"

projectile::projectile(string name, D3DXVECTOR3 pos, float _speed, int _distance, int _damage, D3DClass* _device, CollisionChannel _channel, ColliderType coll)
	:gameObject(name, pos,_channel, coll)
{
	D3DXVECTOR3 position = GetPosition();
	objType = AUTOMOVE;
	SetScale(D3DXVECTOR3(0.5, 0.5, 0.5));
	distance = _distance;
	speed = _speed;
	damage = _damage;
	device = _device;

	InitializeProjectile();
}

void projectile::InitializeProjectile()
{
	bool result;
	result = m_model->Initialize(device->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
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

