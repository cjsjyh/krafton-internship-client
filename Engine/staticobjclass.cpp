#include "staticobjclass.h"
staticobjclass::staticobjclass(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel)
	:gameObject(objname, model, col, _channel)
{
	objType = UNMOVABLE;
}
staticobjclass::~staticobjclass()
{

}
void staticobjclass::Move()
{
	cout << "Static!" << endl;
	return;
}
bool staticobjclass::CheckDestroy()
{
	return false;
}