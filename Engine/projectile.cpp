#include "projectile.h"

projectile::projectile(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float speed)
	:gameObject(name,model,coll,pos)
{
	float x, y, z;
	GetPosition(x, y, z);
	cout << "x: " + to_string(x) << " y: " + to_string(y) << " z: " + to_string(z) << endl;
}