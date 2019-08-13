#include "stdafx.h"

#include "projectileclass.h"
#include "playerclass.h"
#include "d3dclass.h"
#include "gameObject.h"
#include "modelclass.h"
#include "gameManager.h"
#include "skillpatternclass.h"

#include "socketInterface.h"

#include <cstdlib>
#include <ctime>

#include "bossclass.h"

bossclass::bossclass(int _hp, int _damage, D3DClass* _device, playerclass* _player, ColliderType col)
	:hpobjects("boss", _hp, _device, BOSS, col)
{
	curHp = _hp;
	damage = _hp;
	phase = 1;
	player = _player;

	Initialize();
}
bossclass::~bossclass()
{

}

void bossclass::Initialize()
{
	srand((unsigned int)time(NULL));

	InitializeModels();

	maxHp = socketInterface::bossMaxHp;
}

void bossclass::InitializeModels()
{
	for (int i = 0; i < BOSS_PHASE_NUM; i++)
	{
		string tName = "./data/boss/boss" + to_string(i) + ".png";
		cout << tName << endl;
		ModelClass* temp = new ModelClass();
		temp->Initialize(device->GetDevice(), "./data/plane.txt", stdafx::StringToWchar(tName));

		model_list.push_back(temp);
	}
	m_model = model_list[0];
}

void bossclass::SetGameManager(gameManager* _GM)
{
	GM = _GM;

}


void bossclass::CheckHp()
{
	if (socketInterface::bossHp <= socketInterface::bossPhase3Hp)
	{
		phase = 2;
		m_model = model_list[2];
		SetScale(BOSS_SIZE[1]);
	}
	else if (socketInterface::bossHp <= socketInterface::bossPhase2Hp)
	{
		phase = 1;
		m_model = model_list[1];
		SetScale(BOSS_SIZE[2]);
	}
	else
		phase = 0;
}

void bossclass::Frame(int frame)
{
	vector<projectileclass*> shootBullets;
	
	if (frame % 100 == 0)
	{
		int ChosenIndex = rand() % patternFile[phase].size();
		ActivatePattern(patternFile[phase][ChosenIndex]);
	}
	
	PopQueue(shootBullets);
	CheckHp();

	//register bullets
	for (auto iter = shootBullets.begin(); iter != shootBullets.end(); iter++)
		GM->RegisterObjectToRender(*iter);
}


void bossclass::ActivatePattern(BossPatternFile pat)
{
	vector<D3DXVECTOR3> dirVectors;
	for (int i = 0; i < pat.repeat; i++)
	{
		D3DXMATRIX rotMat;
		float rotAngle = pat.rotAngle * i * 0.0174532925f;;
		D3DXMatrixRotationY(&rotMat, rotAngle);

		D3DXVECTOR3 dirVec = player->GetPosition() - GetPosition();
		D3DXVec3TransformCoord(&dirVec, &dirVec, &rotMat);

		dirVectors = skillpatternclass::FireInFan(pat.dirCount, pat.angleBetw, dirVec);
		FireDirections(dirVectors, pat.delay * i, pat.life);
	}

}

void bossclass::Fire(int delay)
{
	projectileclass* temp = GM->GetFromBossPool();
	if (!temp)
	{
		temp = new projectileclass("bossbullet", GetPosition(), 1, 3, device, gameObject::BOSS_BULLET);
		temp->SetDirVector(skillpatternclass::FireAt(GetPosition(), player->GetPosition()));
		temp->AdjustPosition(temp->GetDirVector());
	}
	else
	{
		SetBullet(temp, skillpatternclass::FireAt(GetPosition(), player->GetPosition()));
	}
	

	PushQueue(temp, delay);
}

void bossclass::FireDirections(vector<D3DXVECTOR3> dirVectors, int delay, int distance)
{
	for (auto iter = dirVectors.begin(); iter != dirVectors.end(); iter++)
	{
		projectileclass* temp = GM->GetFromBossPool();
		if (!temp)
		{
			temp = new projectileclass("bossbullet", GetPosition() + 1.5*(*iter), 1, 3, device, gameObject::BOSS_BULLET, distance);
			temp->SetDirVector(*iter);
		}
		else
		{
			SetBullet((projectileclass*)temp, *iter, distance);
		}
		PushQueue(temp, delay);
	}
}

void bossclass::SetBullet(projectileclass* bullet, D3DXVECTOR3 dirVec, int distance)
{
	bullet->SetPosition(GetPosition() + 1.5*dirVec);
	bullet->SetDirVector(dirVec);
	bullet->SetDistance(distance);
	bullet->SetDelay(20);
}


void bossclass::PushQueue(projectileclass* bullet, int frame)
{
	BulletQueue newBullet;
	newBullet.bullet = bullet;
	newBullet.frame = frame;
	bossBullets.push_back(newBullet);
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
