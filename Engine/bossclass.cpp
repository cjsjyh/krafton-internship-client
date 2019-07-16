#include "stdafx.h"

#include "projectileclass.h"
#include "playerclass.h"
#include "d3dclass.h"
#include "gameObject.h"

#include "bossclass.h"

bossclass::bossclass(int _hp, int _damage, D3DClass* _device, playerclass* _player, ColliderType col)
	:hpobjects("boss", _hp, _device, HIT_BOSS, col)
{
	hp = _hp;
	damage = _hp;
	phase = 1;
	player = _player;
}
bossclass::~bossclass()
{

}

vector<projectileclass*> bossclass::Frame(int frame)
{
	vector<projectileclass*> bossBullets;

	if (frame % 60 == 0)
	{
		//bossBullets.push_back(Fire());
	}

	return bossBullets;
}

projectileclass* bossclass::Fire()
{
	projectileclass* temp = new projectileclass("bullet", GetPosition(), 1, 100, 3, device,gameObject::HIT_PLAYER);
	temp->SetDirVector(normalizeVec3(player->GetPosition() - GetPosition()));

	return temp;
}

void bossclass::Fire8Direction()
{
	for (int i = 0; i < 8; i++)
	{
		projectileclass* temp = new projectileclass("bullet", GetPosition(), 1, 100, 3, device, gameObject::HIT_PLAYER);
		//temp->SetDirVector(normalizeVec3());
		PushQueue(temp);
	}
}


void bossclass::PushQueue(projectileclass* bullet)
{
	bossBullets.push_back(bullet);
}

void bossclass::CheckQueue()
{

}

void bossclass::PopQueue()
{

}
