#include "playerclass.h"

playerclass::playerclass(int _hp, D3DClass* _device, D3DXVECTOR3 pos)
	:hpobjects("player", _hp, _device, HIT_PLAYER)
{
	objType = MOVEABLE;
	direction = 1;
}

playerclass::~playerclass()
{

}

projectile* playerclass::Fire(D3DXVECTOR3 dirVec)
{
	projectile* temp = new projectile("bullet", GetPosition(), 1, 100, 3, device, gameObject::HIT_BOSS);
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

void playerclass::SetDirection(char* keys)
{
	// 0 1 2
	// 7   3
	// 6 5 4
	//0 = W A
	if (keys[0] && keys[1])
		direction = 0;
	//2 = W D
	else if (keys[0] && keys[3])
		direction = 2;
	//4 = S D
	else if (keys[2] && keys[3])
		direction = 4;
	//6 = S A
	else if (keys[2] && keys[1])
		direction = 6;
	//1 = W
	else if (keys[0])
		direction = 1;
	//3 = D
	else if (keys[3])
		direction = 3;
	//5 = S
	else if (keys[2])
		direction = 5;
	//7 = A
	else if (keys[1])
		direction = 7;

	SetImage();

	return;
}

void playerclass::Move(char* keys)
{
	SetDirection(keys);

	for (int i = 0; i < sizeof(keys); i++)
	{
		switch (keys[i]) {
		case 'A':
			AdjustPosition(D3DXVECTOR3(-PLAYER_SPEED, 0, 0));
			break;
		case 'S':
			AdjustPosition(D3DXVECTOR3(0, 0, -PLAYER_SPEED));
			break;
		case 'D':
			AdjustPosition(D3DXVECTOR3(PLAYER_SPEED, 0, 0));
			break;
		case 'W':
			AdjustPosition(D3DXVECTOR3(0, 0, PLAYER_SPEED));
			break;
		}
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