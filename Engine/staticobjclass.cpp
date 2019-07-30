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
}

staticobjclass::~staticobjclass()
{

}

void staticobjclass::InitializeStatic2D(string prefix)
{
	string tName = "./data/gameobject/" + prefix + GetName() + ".png";
	bool result = m_model->Initialize(device->GetDevice(), "./data/plane.txt", stdafx::StringToWchar(tName));
}

void staticobjclass::InitializeStatic3D(string prefix)
{
	string tName = "./data/gameobject/" + prefix + GetName() + ".png";
	m_model->Initialize(device->GetDevice(), "./data/cube.txt", stdafx::StringToWchar(tName));
}

void staticobjclass::InitializeStatic2DItem(string prefix)
{
	string tName = "./data/item/" + prefix + GetName() + ".png";
	bool result = m_model->Initialize(device->GetDevice(), "./data/plane.txt", stdafx::StringToWchar(tName));
}

void staticobjclass::InitializeStatic2DItem(string prefix, string fname)
{
	string tName = "./data/item/" + prefix + fname + ".png";
	bool result = m_model->Initialize(device->GetDevice(), "./data/plane.txt", stdafx::StringToWchar(tName));
}


bool staticobjclass::CheckDestroy()
{
	return false;
}