#include "stdafx.h"

#include "gameObject.h"
#include "d3dclass.h"
#include "modelclass.h"

#include "staticobjclass.h"

staticobjclass::staticobjclass(string objname, D3DClass* _device, CollisionChannel _channel, ColliderType _col)
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
	string tName = "../Engine/data/" + GetName() + ".png";
	m_model->Initialize(device->GetDevice(), "../Engine/data/cube.txt", StringToWchar(tName));
}


bool staticobjclass::CheckDestroy()
{
	return false;
}