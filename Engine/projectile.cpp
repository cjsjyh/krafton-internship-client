#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float _speed, gameObject::CollisionChannel _channel)
	:gameObject(name,model,coll,pos,_channel)
{
	D3DXVECTOR3 position;
	GetPosition(position);
	//cout << "x: " + to_string(position.x) << " y: " + to_string(position.y) << " z: " + to_string(position.z) << endl;

	speed = _speed;
	dirVector = D3DXVECTOR3(1, 0, 0);
	//GetDirVector();
}

bool projectile::checkDistance()
{
	if (distance-- > 0)
		return true;
	return false;
}

void projectile::GetDirVector()
{
	D3DXMATRIX *temp;
	D3DXMatrixIdentity(temp);
	D3DXMatrixRotationY(temp, rot.y);
	D3DXVec3TransformCoord(new D3DXVECTOR3(1,0,0), &dirVector, temp);

	cout << "x: " + to_string(dirVector.x) << "y: " + to_string(dirVector.y) << "z: " + to_string(dirVector.z) << endl;
}
