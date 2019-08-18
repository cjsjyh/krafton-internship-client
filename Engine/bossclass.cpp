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

bossclass::bossclass(int _hp, int _damage, D3DClass* _device, vector<playerclass*> _player, ColliderType col)
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
	
	while (socketInterface::bossPatternQueue.size() > 0){
		if (socketInterface::bossPatternFrame.front() > socketInterface::frame)
		{
			printf("Frame not ready yet! PatterFrame: %d CurrentFrame: %d\n",socketInterface::bossPatternFrame.front(),socketInterface::frame);
			printf("bullet queue size: %d\n", bossBullets.size());
			break;
		}
		printf("Handling Pattern id: %d\n",socketInterface::bossPatternQueue.front());
		socketInterface::bossPatternFrame.pop();

		int ChosenIndex = socketInterface::bossPatternQueue.front();
		socketInterface::bossPatternQueue.pop();
		
		ActivatePattern(patternFile[phase][ChosenIndex], socketInterface::bossPatternTarget.front());
		socketInterface::bossPatternTarget.pop();
	}
	
	PopQueue(shootBullets);
	CheckHp();

	//쏠 총알이 하나도 없는 경우 디폴트를 넣음
	/*if (shootBullets.size() == 0) {
		ActivatePattern(patternFile[phase][0]);
		PopQueue(shootBullets);
	}*/

	//register bullets
	for (auto iter = shootBullets.begin(); iter != shootBullets.end(); iter++)
		GM->RegisterObjectToRender(*iter);
}


void bossclass::ActivatePattern(BossPatternFile pat, int targetId)
{
	vector<D3DXVECTOR3> dirVectors;
	for (int i = 0; i < pat.repeat; i++)
	{
		D3DXMATRIX rotMat;
		float rotAngle = pat.rotAngle * i * 0.0174532925f;;
		D3DXMatrixRotationY(&rotMat, rotAngle);

		D3DXVECTOR3 dirVec = player[targetId]->GetPosition() - GetPosition();
		D3DXVec3TransformCoord(&dirVec, &dirVec, &rotMat);

		dirVectors = skillpatternclass::FireInFan(pat.dirCount, pat.angleBetw, dirVec);
		FireDirections(dirVectors, pat.delay * i, pat.life, pat.bullet_slow_frame, pat.bullet_speed, pat.bullet_type);
	}

}

void bossclass::FireDirections(vector<D3DXVECTOR3> dirVectors, int fireDelay, int distance, int slowFrame, float bulletSpeed, int bulletType)
{
	for (auto iter = dirVectors.begin(); iter != dirVectors.end(); iter++)
	{
		projectileclass* temp = GM->GetFromBossPool();
		if (!temp)
		{
			temp = new projectileclass("bossbullet", GetPosition() + 1.5*(*iter), bulletSpeed, 3, device, gameObject::BOSS_BULLET, distance,slowFrame);
			temp->type = bulletType;
			temp->SetDirVector(*iter);
		}
		else
		{
			SetBullet((projectileclass*)temp, *iter, distance, slowFrame, bulletSpeed, bulletType);
		}
		PushQueue(temp, fireDelay);
	}
}

void bossclass::SetBullet(projectileclass* bullet, D3DXVECTOR3 dirVec, int distance, int slowFrame, float bulletSpeed, int bulletType)
{
	bullet->SetPosition(GetPosition() + 1.5*dirVec);
	bullet->SetDelay(slowFrame);
	bullet->SetSpeed(bulletSpeed);
	bullet->SetDirVector(dirVec);
	bullet->SetDistance(distance);
	bullet->type = bulletType;
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
		if (bossBullets[i].frame-- <= 0)
		{
			shootBullet.push_back(bossBullets[i].bullet);
			bossBullets.erase(bossBullets.begin() + i);
		}
	}
}
