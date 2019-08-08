#include "stdafx.h"

#include "projectileclass.h"
#include "playerclass.h"
#include "d3dclass.h"
#include "gameObject.h"
#include "modelclass.h"
#include "gameManager.h"
#include "skillpatternclass.h"

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
	SetBossPhasePattern();

	InitializeModels();
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
	if ((float)curHp / maxHp < BOSS_PHASE3_HP)
	{
		phase = 2;
		m_model = model_list[2];
		SetScale(BOSS_SIZE[1]);
	}
	else if ((float)curHp / maxHp < BOSS_PHASE2_HP)
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
	if (frame % 60 == 0)
	{
		int ChosenIndex = rand() % bossPatternPool[phase].size();
		ActivatePattern(bossPatternPool[phase][ChosenIndex]);
	}
	PopQueue(shootBullets);
	CheckHp();

	//register bullets
	for (auto iter = shootBullets.begin(); iter != shootBullets.end(); iter++)
		GM->RegisterObjectToRender(*iter);
}


void bossclass::SetBossPhasePattern()
{
	for (int i = 0; i < BOSS_PHASE_NUM; i++)
	{
		vector<BossPattern> temp;
		bossPatternPool.push_back(temp);
	}
	BossPattern p1_1;
	p1_1.type = FIRE_AT_PLAYER;
	BossPattern p1_2;
	p1_2.type = FIRE_IN_FAN;
	p1_2.dirCount = 3;
	p1_2.Angle = 10;
	bossPatternPool[0].push_back(p1_1);
	bossPatternPool[0].push_back(p1_2);

	BossPattern p2_1;
	p2_1.type = FIRE_AT_PLAYER;
	BossPattern p2_2;
	p2_2.type = FIRE_IN_FAN;
	p2_2.dirCount = 5;
	p2_2.Angle = 10;
	bossPatternPool[1].push_back(p2_1);
	bossPatternPool[1].push_back(p2_2);

	BossPattern p3_1;
	p3_1.type = FIRE_IN_FAN;
	p3_1.dirCount = 7;
	p3_1.Angle = 5;
	BossPattern p3_2;
	p3_2.type = FIRE_ALL_DIR;
	p3_2.dirCount = 10;
	bossPatternPool[2].push_back(p3_1);
	bossPatternPool[2].push_back(p3_2);
}


void bossclass::ActivatePattern(BossPattern pat)
{
	vector<D3DXVECTOR3> dirVectors;
	switch (pat.type)
	{
	case FIRE_AT_PLAYER:
		Fire();
		break;
	case FIRE_IN_FAN:
		dirVectors = skillpatternclass::FireInFan(pat.dirCount, pat.Angle, GetPosition(), player->GetPosition());
		FireDirections(dirVectors);
		break;
	case FIRE_ALL_DIR:
		dirVectors = skillpatternclass::FireInCircle(pat.dirCount);
		for(int i=0;i<10;i++)
			FireDirections(dirVectors, 40*i);
		break;
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

void bossclass::FireDirections(vector<D3DXVECTOR3> dirVectors, int delay)
{
	for (auto iter = dirVectors.begin(); iter != dirVectors.end(); iter++)
	{
		projectileclass* temp = GM->GetFromBossPool();
		if (!temp)
		{
			temp = new projectileclass("bossbullet", GetPosition() + 1.5*(*iter), 1, 3, device, gameObject::BOSS_BULLET);
			temp->SetDirVector(*iter);
		}
		else
		{
			SetBullet((projectileclass*)temp, *iter);
		}
		PushQueue(temp, delay);
	}
}

void bossclass::SetBullet(projectileclass* bullet, D3DXVECTOR3 dirVec)
{
	bullet->SetPosition(GetPosition() + 1.5*dirVec);
	bullet->SetDirVector(dirVec);
	bullet->SetDistance(100);
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
