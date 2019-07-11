#include "playerclass.h"

playerclass::playerclass(ModelClass* model, int hp, D3DXVECTOR3 pos)
	:hpobjects("player", model, hp, HIT_PLAYER)
{
	objType = MOVEABLE;
	direction = 1;
}

playerclass::~playerclass()
{

}

int playerclass::GetDirection()
{
	return direction;
}

void playerclass::SetDirection(int _dir)
{
	direction = _dir;
}