#include "playerclass.h"

playerclass::playerclass(ModelClass* model, D3DXVECTOR3 pos)
	:gameObject("player", model, gameObject::COLLIDER_BOX, pos)
{
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