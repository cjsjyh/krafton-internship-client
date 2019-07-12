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

void playerclass::SetDirection(int _dir)
{
	direction = _dir;
}