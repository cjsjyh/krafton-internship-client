#include "stdafx.h"

#include "projectileclass.h"
#include "d3dclass.h"
#include "modelclass.h"
#include "gameObject.h"
#include "gameManager.h"
#include "inputclass.h"

#include "playerclass.h"

playerclass::playerclass(int _hp, D3DClass* _device, D3DXVECTOR3 pos)
	:hpobjects("player", _hp, _device, PLAYER)
{
	objType = MOVEABLE;
	direction = 1;

	PLAYER_DASH_SPEED = 2;
	PLAYER_DASH_FRAME = 20;
	PLAYER_DASH_PAUSE_FRAME = 5;

	dashFrame = dashPauseFrame = -1;
	dashDir = -1;

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
		string tName = "../Engine/data/player/player" + to_string(i) + ".png";
		ModelClass* temp = new ModelClass();
		temp->Initialize(device->GetDevice(), "../Engine/data/plane.txt", StringToWchar(tName));

		model_list.push_back(temp);
	}
	m_model = model_list[1];
	
	
}

void playerclass::SetGameManager(gameManager* _GM)
{
	GM = _GM;
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
	bullet->SetDistance(100);
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

void playerclass::Frame(int* keys, int frame)
{
	SetDirection(keys);
	
	//Dashing
	if (Dash(keys, frame));
	else if(InputClass::IsWASDKeyPressed(keys))
		AdjustPosition(GetDirectionVector(direction) * PLAYER_SPEED);

	ObjectInteraction(keys);
}

bool playerclass::Dash(int* keys, int frame)
{
	if (keys[4] == DIK_SPACE || dashFrame != -1)
	{
		if (dashFrame == -1)
		{
			dashFrame = frame;
			dashDir = direction;
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

int playerclass::ObjectInteraction(int* keys)
{
	if (keys[6] == DIK_F)
	{
		gameObject* hitObj = GM->CheckInteraction(GetPosition(), PLAYER_INTERACTION_RANGE);

		if (!hitObj)
			cout << "null!" << endl;
		else
			cout << "hiit obj: " + hitObj->GetName() << endl;
	}

	return 1;
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