#include "projectileclass.h"

projectileclass::projectileclass(string name, ModelClass* model, ColliderType coll, D3DXVECTOR3 pos, float speed)
	:gameObject(name,model,coll,pos)
{
	float x, y, z;
	GetPosition(x, y, z);
	cout << x << y << z << endl;
}