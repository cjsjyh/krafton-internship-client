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

	cout << "player direction" << to_string(direction) << endl;
	SetImage();

	return;
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