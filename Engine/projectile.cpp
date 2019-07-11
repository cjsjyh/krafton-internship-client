#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float _speed, CollisionChannel _channel, ObjectType _objType)
	:gameObject(name,model,coll,pos,_channel,_objType)
{
	D3DXVECTOR3 position = GetPosition();
	//cout << "x: " + to_string(position.x) << " y: " + to_string(position.y) << " z: " + to_string(position.z) << endl;
	speed = _speed;
	SetScale(D3DXVECTOR3(0.5, 0.5, 0.5));
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

void projectile::Move()
{
	cout << "Move!" << endl;
}

