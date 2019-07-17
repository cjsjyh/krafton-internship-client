#include "stdafx.h"

#include "projectileclass.h"
#include "d3dclass.h"
#include "modelclass.h"
#include "gameObject.h"

#include "playerclass.h"

playerclass::playerclass(int _hp, D3DClass* _device, D3DXVECTOR3 pos)
	:hpobjects("player", _hp, _device, HIT_PLAYER)
{
	objType = MOVEABLE;
	direction = 1;

	PLAYER_DASH_SPEED = 2;
	PLAYER_DASH_FRAME = 20;
	PLAYER_DASH_PAUSE_FRAME = 5;

	dashFrame = dashPauseFrame = -1;
	dashDir = -1;

	InitializeModels();
}

playerclass::~playerclass()
{

}

void playerclass::InitializeModels()
{
	for (int i = 0; i < PLAYER_IMAGE_NUM; i++)
	{
		string tName = "../Engine/data/player/player" + to_string(i) + ".dds";
		cout << tName << endl;
		ModelClass* temp = new ModelClass();
		temp->Initialize(device->GetDevice(), "../Engine/data/plane.txt", StringToWchar(tName));

		model_list.push_back(temp);
	}
	m_model = model_list[1];
	
	
}

projectileclass* playerclass::Fire(D3DXVECTOR3 dirVec)
{
	projectileclass* temp = new projectileclass("bullet", GetPosition(), 1, 3, device, 0, 100 ,gameObject::HIT_BOSS);
	temp->SetDirVector(dirVec);
	return temp;
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
		m_model = model_list[0];
	}
	//2 = W D
	else if (keys[0] && keys[3])
	{
		direction = 2;
		m_model = model_list[2];
	}
	//4 = S D
	else if (keys[2] && keys[3])
	{
		direction = 4;
		m_model = model_list[4];
	}
	//6 = S A
	else if (keys[2] && keys[1])
	{
		direction = 6;
		m_model = model_list[6];
	}
	//1 = W
	else if (keys[0])
	{
		direction = 1;
		m_model = model_list[1];
	}
	//3 = D
	else if (keys[3])
	{
		direction = 3;
		m_model = model_list[3];
	}
	//5 = S
	else if (keys[2])
	{
		direction = 5;
		m_model = model_list[5];
	}
	//7 = A
	else if (keys[1])
	{
		direction = 7;
		m_model = model_list[7];
	}

	SetImage();

	return;
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

void playerclass::Move(int* keys, int frame)
{
	SetDirection(keys);
	
	//Dashing
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
	}
	
	else if(stdafx::IsKeyPressed(keys))
	{
		AdjustPosition(GetDirectionVector(direction) * PLAYER_SPEED);
	}

}

void playerclass::SetImage()
{
	switch (direction)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	default:
		break;
	}

	return;
}