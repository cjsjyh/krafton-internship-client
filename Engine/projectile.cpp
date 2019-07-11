#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float _speed, gameObject::CollisionChannel _channel)
	:gameObject(name,model,coll,pos,_channel)
{
	D3DXVECTOR3 position = GetPosition();
	//cout << "x: " + to_string(position.x) << " y: " + to_string(position.y) << " z: " + to_string(position.z) << endl;

	speed = _speed;
	dirVector = D3DXVECTOR3(1, 0, 0);
	GetDirVector();
}

bool projectile::checkDistance()
{
	if (distance-- > 0)
		return true;
	return false;
}

void projectile::SetDirVector(D3DXVECTOR3 dir)
{
	dirVector = dir * speed;
}

D3DXVECTOR3 projectile::GetDirVector()
{
	/*
	D3DXMATRIX *temp = new D3DXMATRIX;
	D3DXMatrixIdentity(temp);
	D3DXMatrixRotationY(temp, rot.y);
	D3DXVec3TransformCoord(new D3DXVECTOR3(1,0,0), &dirVector, temp);
	*/

	return dirVector;
}


