#include "stdafx.h"

#include "projectileclass.h"
#include "playerclass.h"
#include "d3dclass.h"
#include "gameObject.h"
#include "modelclass.h"
#include "gameManager.h"

#include "bossclass.h"

bossclass::bossclass(int _hp, int _damage, D3DClass* _device, playerclass* _player, ColliderType col)
	:hpobjects("boss", _hp, _device, HIT_BOSS, col)
{
	hp = _hp;
	damage = _hp;
	phase = 1;
	player = _player;

	InitializeModels();
}
bossclass::~bossclass()
{

}

void bossclass::InitializeModels()
{
	for (int i = 0; i < BOSS_IMAGE_NUM; i++)
	{
		string tName = "../Engine/data/boss/boss" + to_string(i) + ".png";
		cout << tName << endl;
		ModelClass* temp = new ModelClass();
		temp->Initialize(device->GetDevice(), "../Engine/data/plane.txt", StringToWchar(tName));

		model_list.push_back(temp);
	}
	m_model = model_list[0];
}

void bossclass::SetGameManager(gameManager* _GM)
{
	GM = _GM;
}


vector<projectileclass*> bossclass::Frame(int frame)
{
	vector<projectileclass*> shootBullets;
	if (frame % 60 == 0)
	{
		FireDirections(10, frame);
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

void bossclass::FireDirections(int dir, int frame)
{
	D3DXMATRIX rotMatrix;
	D3DXVECTOR3 dirVec = D3DXVECTOR3(1, 0, 1);
	D3DXVECTOR4 result;
	float angle;
	
	
	for(int i=0;i < dir; i++)
	{			
		dirVec = D3DXVECTOR3(1, 0, 1);
		angle = 360 / dir * i;
		angle *= 0.0174532925f;

		D3DXMatrixIdentity(&rotMatrix);
		D3DXMatrixRotationY(&rotMatrix, angle);

		D3DXVec3Transform(&result, &dirVec, &rotMatrix);

		dirVec = normalizeVec3(D3DXVECTOR3(result.x, result.y, result.z));

		projectileclass* temp = GM->GetFromBossPool();
		if (!temp)
		{
			//cout << "temp NULL" << endl;
			temp = new projectileclass("bossbullet", GetPosition() + dirVec, 1, 3, device, 5, 100, gameObject::HIT_PLAYER);
			temp->SetDirVector(dirVec);
		}
		else
		{
			cout << "found" << endl;
			SetBullet((projectileclass*)temp, dirVec);
		}

		PushQueue(temp, 0);
	}
	
}

void bossclass::SetBullet(projectileclass* bullet, D3DXVECTOR3 dirVec)
{
	bullet->SetPosition(GetPosition() + dirVec);
	bullet->SetDirVector(dirVec);
	bullet->SetDistance(100);
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
	}
}
