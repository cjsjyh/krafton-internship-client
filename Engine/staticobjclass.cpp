#include "staticobjclass.h"
staticobjclass::staticobjclass(string objname, D3DClass* _device, ColliderType _col, CollisionChannel _channel)
	:gameObject(objname, _channel, _col)
{
	objType = UNMOVABLE;
	device = _device;

	InitializeStatic();
}

staticobjclass::~staticobjclass()
{

}

void staticobjclass::InitializeStatic()
{
	string tName = "../Engine/data/" + GetName() + ".dds";
	m_model->Initialize(device->GetDevice(), "../Engine/data/cube.txt", StringToWchar(tName));
}


bool staticobjclass::CheckDestroy()
{
	return false;
}