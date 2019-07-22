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

void staticobjclass::InitializeStatic2D()
{
	string tName = "../Engine/data/" + GetName() + ".png";
	cout << "texture anem: " + tName << endl;
	bool result = m_model->Initialize(device->GetDevice(), "../Engine/data/plane.txt", stdafx::StringToWchar(tName));
	cout << "intialize result: " + to_string(result) << endl;
}

void staticobjclass::InitializeStatic3D()
{
	string tName = "../Engine/data/" + GetName() + ".png";
	m_model->Initialize(device->GetDevice(), "../Engine/data/cube.txt", stdafx::StringToWchar(tName));
}


bool staticobjclass::CheckDestroy()
{
	return false;
}