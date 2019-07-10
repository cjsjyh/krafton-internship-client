#include "gameObject.h"


gameObject::gameObject(string objname, ModelClass* model, ColliderType col)
{
	pos_x = pos_y = pos_z = 0;
	scale_x = scale_y = scale_z = 1;
	rot_x = rot_y = rot_z = 0;
	w = h = l = 1;
	collider_size = 0.5;

	name = objname;
	m_model = model;
	collider = col;
}

gameObject::gameObject(string objname, ModelClass* model, ColliderType col, D3DXVECTOR3 pos)
{
	pos_z = pos.x;
	pos_y = pos.y;
	pos_z = pos.z;

	scale_x = scale_y = scale_z = 1;
	rot_x = rot_y = rot_z = 0;
	w = h = l = 1;

	name = objname;
	m_model = model;
	collider = col;
}

gameObject::~gameObject()
{

}

ModelClass* gameObject::GetModel()
{
	return m_model;
}

string gameObject::GetName()
{
	return name;
}

gameObject::ColliderType gameObject::GetColliderType()
{
	return collider;
}

void gameObject::GetSize(float& width, float& height, float& length)
{
	width = w;
	height = h;
	length = l;
	return;
}

void gameObject::GetPosition(float& x, float& y, float& z)
{
	x = pos_x;
	y = pos_y;
	z = pos_z;
	return;
}

void gameObject::GetRotation(float& x, float& y, float& z)
{
	x = rot_x;
	y = rot_y;
	z = rot_z;

	return;
}

void gameObject::GetScale (float& x, float& y, float& z)
{
	x = scale_x;
	y = scale_y;
	z = scale_z;

	return;
}

void gameObject::SetPosition(float x, float y, float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	return;
}

void gameObject::SetRotation(float x, float y, float z)
{
	rot_x = x;
	while (rot_x > 360)
		rot_x = rot_x - 360;

	rot_y = y;
	while (rot_y > 360)
		rot_y = rot_y - 360;

	rot_z = z;
	while (rot_z > 360)
		rot_z = rot_z - 360;
	return;
}

void gameObject::SetScale(float x, float y, float z)
{
	//width
	scale_x = x;
	w *= x;

	//height
	scale_y = y;
	h *= y;

	//length
	scale_z = z;
	l *= z;
	
	collider_size = min(min(x, y), z) * 0.75;
	return;
}

void gameObject::AdjustPosition(float x, float y, float z)
{
	pos_x += x;
	pos_y += y;
	pos_z += z;
	return;
}

void gameObject::AdjustRotation(float x, float y, float z)
{
	rot_x += x;
	while (rot_x > 360)
		rot_x = rot_x - 360;

	rot_y += y;
	while (rot_y > 360)
		rot_y = rot_y - 360;

	rot_z += z;
	while (rot_z > 360)
		rot_z = rot_z - 360;
	return;
}

void gameObject::AdjustScale(float x, float y, float z)
{
	scale_x += x;
	w += x * 2;

	scale_y += y;
	h += y * 2;

	scale_z += z;
	l += z * 2;
}

void gameObject::GetWorldMatrix(D3DXMATRIX& WorldMatrix)
{
	D3DXMATRIX temp;
	
	D3DXMatrixIdentity(&WorldMatrix);

	D3DXMatrixScaling(&temp, scale_x, scale_y, scale_z);
	WorldMatrix *= temp;

	D3DXMatrixRotationX(&temp, rot_x);
	WorldMatrix *= temp;
	D3DXMatrixRotationY(&temp, rot_y);
	WorldMatrix *= temp;
	D3DXMatrixRotationZ(&temp, rot_z);
	WorldMatrix *= temp;

	D3DXMatrixTranslation(&temp, pos_x, pos_y, pos_z);
	WorldMatrix *= temp;
}