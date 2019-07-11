#include "staticobjclass.h"
staticobjclass::staticobjclass(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel, ObjectType type)
	:gameObject(objname, model, col, _channel, type)
{

}
staticobjclass::~staticobjclass()
{

}
void staticobjclass::Move()
{
	cout << "Static!" << endl;
	return;
}