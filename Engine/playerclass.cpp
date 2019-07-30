#include "stdafx.h"

#include "projectileclass.h"
#include "d3dclass.h"
#include "modelclass.h"
#include "gameObject.h"
#include "gameManager.h"
#include "inputclass.h"
#include "itemmanagerclass.h"
#include "skillpatternclass.h"

#include "playerclass.h"

playerclass::playerclass(int _hp, D3DClass* _device, D3DXVECTOR3 pos)
	:hpobjects("player", _hp, _device, PLAYER)
{
	objType = MOVEABLE;
	channel = gameObject::PLAYER;
	direction = 1;

	lastLeftClick = 0;
	PLAYER_DASH_SPEED = 2;
	PLAYER_DASH_FRAME = 20;
	PLAYER_DASH_PAUSE_FRAME = 5;
	PLAYER_BULLET_RELOAD = 10;
	PLAYER_BULLET_COUNT = 1;
	PLAYER_BULLET_ANGLE = 0;

	dashFrame = dashPauseFrame = -1;
	dashDir = -1;
	attackType = "basic";

	playerPosSave.push_back(D3DXVECTOR3(0, 0, 0));
	InitializeModels();
}

playerclass::~playerclass()
{

}

void playerclass::InitializeModels()
{
	for (int i = 0; i < PLAYER_IMAGE_NUM; i++)
	{
		string tName = "./data/player/player" + to_string(i) + ".png";
		ModelClass* temp = new ModelClass();
		temp->Initialize(device->GetDevice(), "./data/plane.txt", stdafx::StringToWchar(tName));

		model_list.push_back(temp);
	}
	m_model = model_list[1];
	
	
}

void playerclass::SetManager(gameManager* _GM, itemmanagerclass* _IM)
{
	GM = _GM;
	IM = _IM;
}

projectileclass* playerclass::Fire(D3DXVECTOR3 dirVec)
{
	projectileclass* temp = GM->GetFromPlayerPool();
	if (!temp)
	{
		temp = new projectileclass("playerbullet", GetPosition(), PLAYER_BULLET_SPEED,
									PLAYER_BULLET_DAMAGE, device, gameObject::PLAYER_BULLET,PLAYER_BULLET_DELAY,
									PLAYER_BULLET_DISTANCE);
		temp->SetDirVector(dirVec);
	}
	else
	{
		SetBullet((projectileclass*)temp, dirVec);
	}
		
	return temp;
}

void playerclass::SetBullet(projectileclass* bullet, D3DXVECTOR3 dirVec)
{
	bullet->SetPosition(GetPosition() + dirVec);
	bullet->SetDirVector(dirVec);
	bullet->SetDistance(PLAYER_BULLET_DISTANCE);
}

void playerclass::SetSpeed(float _speed)
{
	PLAYER_SPEED = _speed;
	return;
}

int playerclass::GetDirection()
{
	return direction;
}

void playerclass::SetDirection(int* keys)
{
	// 0 1 2
	// 7   3
	// 6 5 4

	//0 = W A
	if (keys[0] && keys[1])
	{
		direction = 0;
	}
	//2 = W D
	else if (keys[0] && keys[3])
	{
		direction = 2;
	}
	//4 = S D
	else if (keys[2] && keys[3])
	{
		direction = 4;
	}
	//6 = S A
	else if (keys[2] && keys[1])
	{
		direction = 6;
	}
	//1 = W
	else if (keys[0])
	{
		direction = 1;
	}
	//3 = D
	else if (keys[3])
	{
		direction = 3;
	}
	//5 = S
	else if (keys[2])
	{
		direction = 5;
	}
	//7 = A
	else if (keys[1])
	{
		direction = 7;
	}

	SetImage();
	return;
}
void playerclass::SetDirection(int dir)
{
	direction = dir;
}

D3DXVECTOR3 playerclass::GetDirectionVector(int dir)
{
	// 0 1 2
	// 7   3
	// 6 5 4
	switch (dir)
	{
	case 0:
		return normalizeVec3(D3DXVECTOR3(-1, 0, 1));
	case 1:
		return normalizeVec3(D3DXVECTOR3(0, 0, 1));
	case 2:
		return normalizeVec3(D3DXVECTOR3(1, 0, 1));
	case 3:
		return normalizeVec3(D3DXVECTOR3(1, 0, 0));
	case 4:
		return normalizeVec3(D3DXVECTOR3(1, 0, -1));
	case 5:
		return normalizeVec3(D3DXVECTOR3(0, 0, -1));
	case 6:
		return normalizeVec3(D3DXVECTOR3(-1, 0, -1));
	case 7:
		return normalizeVec3(D3DXVECTOR3(-1, 0, 0));
	}
}

void playerclass::Frame(int* keys, bool* mousePress, D3DXVECTOR3 vecToMouse, int frame)
{
	//CAN CLICK AGAIN!
	if (frame - lastLeftClick > PLAYER_BULLET_RELOAD)
		lastLeftClick = 0;

	if (InputClass::LeftMouseClicked(mousePress))
	{
		if (!lastLeftClick)
		{
			vector<D3DXVECTOR3> temp = skillpatternclass::FireInFan(PLAYER_BULLET_COUNT, PLAYER_BULLET_ANGLE, D3DXVECTOR3(0,0,0), vecToMouse);
			for (auto iter = temp.begin(); iter != temp.end(); iter++)
			{
				GM->RegisterObjectToRender(Fire(*iter), GM->scene);
			}
			
			lastLeftClick = frame;
		}
	}

	SetDirection(keys);

	//Dashing
	if (Dash(keys, frame));
	else if (InputClass::IsWASDKeyPressed(keys))
	{
		D3DXVECTOR3 nextPos = GetPosition() + GetDirectionVector(direction) * PLAYER_SPEED;
		if(GM->CheckMovable(nextPos, GetCollSize()))
			if(GM->CheckMapOut(nextPos))
				AdjustPosition(GetDirectionVector(direction) * PLAYER_SPEED);
	}
	
	if (GM->scene == 1)
	{
		NotifyObjectTooltip();

		if (InputClass::IsKeyPressed(keys, DIK_RETURN))
		{
			ObjectInteraction();
		}
	}
}

bool playerclass::Dash(int* keys, int frame)
{
	if (keys[4] == DIK_SPACE || dashFrame != -1)
	{
		//START DASH
		if (dashFrame == -1)
		{
			dashFrame = frame;
			dashDir = direction;
			channel = gameObject::NO_COLLISION;
		}
		else
		{
			//DASH FINISH
			if (frame - dashFrame >= PLAYER_DASH_FRAME)
			{
				//DASH PAUSE START
				if (dashPauseFrame == -1)
				{
					dashPauseFrame = frame;
					channel = gameObject::PLAYER;
				}
				else
				{
					//DASH CYCLE DONE
					if (frame - dashPauseFrame >= PLAYER_DASH_PAUSE_FRAME)
					{
						dashFrame = -1;
						dashPauseFrame = -1;
					}
				}
			}
			//KEEP ON DASHING
			else
			{
				AdjustPosition(GetDirectionVector(dashDir) * PLAYER_DASH_SPEED);
			}
		}
		return true;
	}
	return false;
}

int playerclass::ObjectInteraction()
{
	gameObject* hitObj = GM->CheckInteraction(GetPosition(), PLAYER_INTERACTION_RANGE);

	if (hitObj)
	{
		IM->SetItemUsed(hitObj->GetName(), 0);
	}
	
	return 1;
}

void playerclass::NotifyObjectTooltip()
{
	gameObject* hitObj = GM->CheckInteraction(GetPosition(), PLAYER_INTERACTION_RANGE);
	if (hitObj)
	{
		IM->DisplayTooltip(hitObj);
	}
}


void playerclass::SetImage()
{
	m_model = model_list[direction];
}

void playerclass::SavePlayerPos(int scene)
{
	int curSize = playerPosSave.size();
	if (curSize < scene+1)
	{
		for (int i = curSize; i < scene + 1; i++)
			playerPosSave.push_back(D3DXVECTOR3(0, 0, 0));
	}
	playerPosSave[scene] = GetPosition();
}

D3DXVECTOR3 playerclass::GetSavedPlayerPos(int scene)
{
	int curSize = playerPosSave.size();
	if (curSize < scene)
	{
		for (int i = curSize; i < scene + 1; i++)
			playerPosSave.push_back(D3DXVECTOR3(0, 0, 0));
	}
	return playerPosSave[scene];
}

void playerclass::SetPlayerAttackType(string _type)
{
	attackType = _type;
}

void playerclass::AddPlayerItem(string _item)
{
	playerItems.push_back(_item);
}

int playerclass::GetPlayerItemSize()
{
	return playerItems.size();
}

string playerclass::GetPlayerItem(int i)
{
	return playerItems[i];
}