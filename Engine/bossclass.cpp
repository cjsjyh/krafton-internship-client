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
	vector<projectileclass*> shootBullets;
	if (frame % 60 == 0)
	{
		Fire8Direction(frame);
		//bossBullets.push_back(Fire());
	}
	PopQueue(shootBullets);
	return shootBullets;
}

projectileclass* bossclass::Fire()
{
	projectileclass* temp = new projectileclass("bullet", GetPosition(), 1, 3, device, 50, 100,gameObject::HIT_PLAYER);
	temp->SetDirVector(normalizeVec3(player->GetPosition() - GetPosition()));
	temp->AdjustPosition(temp->GetDirVector());

	return temp;
}

void bossclass::Fire8Direction(int frame)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//(0,0,0) ¹æÇâ 
			if (i == 1 && j == 1)
				continue;
			
			D3DXVECTOR3 dir = normalizeVec3(D3DXVECTOR3(-1 + i, 0, -1 + j));
			projectileclass* temp = new projectileclass("bullet", GetPosition()+dir, 1, 3, device, 50, 100,gameObject::HIT_PLAYER);
			temp->SetDirVector(dir);
			temp->AdjustPosition(temp->GetDirVector());
			
			PushQueue(temp, 0);
		}
	}
}


void bossclass::PushQueue(projectileclass* bullet, int frame)
{
	BulletQueue newBullet;
	newBullet.bullet = bullet;
	newBullet.frame = frame;
	bossBullets.push_back(newBullet);
}

void bossclass::CheckQueue()
{
	
}

void bossclass::PopQueue(vector<projectileclass*>& shootBullet)
{
	for (int i = bossBullets.size() -1; i >= 0; i--)
	{
		bossBullets[i].frame--;
		if (bossBullets[i].frame <= 0)
		{
			shootBullet.push_back(bossBullets[i].bullet);
			bossBullets.erase(bossBullets.begin() + i);
		}
		cout << to_string(i) << endl;
	}
}
