#include "playerclass.h"

playerclass::playerclass(ModelClass* model, D3DXVECTOR3 pos)
	:gameObject("player", model, gameObject::COLLIDER_BOX, pos,HIT_PLAYER)
{
	objType = MOVEABLE;
	direction = 1;
}

playerclass::~playerclass()
{

}

void playerclass::Move()
{
	cout << "player move!" << endl;
	return;
}

bool playerclass::CheckDestroy()
{
	return false;
}


int playerclass::GetDirection()
{
	return direction;
}

void playerclass::SetDirection(int _dir)
{
	direction = _dir;
}