#include "gameObject.h"


gameObject::gameObject()
{
	pos_x = pos_y = pos_z = 0;
	scale_x = scale_y = scale_z = 1;
	rot_x = rot_y = rot_z = 0;
}

gameObject::gameObject(float x, float y, float z)
{
	pos_z = x;
	pos_y = y;
	pos_z = z;

	scale_x = scale_y = scale_z = 1;
	rot_x = rot_y = rot_z = 0;
}

gameObject::~gameObject()
{

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
	scale_x = x;
	scale_y = y;
	scale_z = z;
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
	scale_y += y;
	scale_z += z;
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