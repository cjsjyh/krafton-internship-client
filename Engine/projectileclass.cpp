#include "stdafx.h"

#include "modelclass.h"
#include "d3dclass.h"

#include "projectileclass.h"

projectileclass::projectileclass(string name, D3DXVECTOR3 pos, float _speed, int _damage, D3DClass* _device, CollisionChannel _channel, int _distance, int _delay,  ColliderType coll)
	:gameObject(name, pos,_channel, coll)
{
	D3DXVECTOR3 position = GetPosition();
	objType = AUTOMOVE;
	SetScale(D3DXVECTOR3(0.5, 0.5, 0.5));
	distance = _distance;
	speed = _speed;
	damage = _damage;
	device = _device;
	delay = _delay;

	InitializeProjectile();
}

projectileclass::~projectileclass()
{

}

void projectileclass::InitializeProjectile()
{
	bool result;
	//result = m_model->Initialize(device->GetDevice(), "./data/plane.txt", L"./data/gameobject/bullet.png");
}

bool projectileclass::checkDistance()
{
	if (distance-- < 0)
		return true;
	return false;
}

int projectileclass::GetFrame()
{
	return distance;
}

void projectileclass::SetDistance(int _dist)
{
	distance = _dist;
}

void projectileclass::SetDirVector(D3DXVECTOR3 dir)
{
	dirVector = dir * speed;
}

D3DXVECTOR3 projectileclass::GetDirVector()
{
	return dirVector;
}

void projectileclass::SetDelay(int _delay)
{
	delay = _delay;
}

void projectileclass::SetSpeed(float _speed)
{
	speed = _speed;
}

void projectileclass::Move(float multiplier)
{
	AdjustPosition(dirVector * multiplier);
}

bool projectileclass::CheckDestroy()
{
	return checkDistance();
}

