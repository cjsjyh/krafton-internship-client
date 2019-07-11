#include "staticobjclass.h"
staticobjclass::staticobjclass(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel)
	:gameObject(objname, model, _channel)
{
	objType = UNMOVABLE;
}
staticobjclass::~staticobjclass()
{

}
bool staticobjclass::CheckDestroy()
{
	return false;
}