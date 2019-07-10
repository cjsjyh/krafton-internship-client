#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float speed)
	:gameObject(name,model,coll,pos)
{
	D3DXVECTOR3 position;
	GetPosition(position);
	cout << "x: " + to_string(position.x) << " y: " + to_string(position.y) << " z: " + to_string(position.z) << endl;
}