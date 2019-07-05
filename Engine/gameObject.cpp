#include "gameObject.h"


gameObject::gameObject()
{
	pos_x = pos_y = pos_z = 0;
	rot = 0;
	height = width = 0;
}

gameObject::gameObject(float w, float h)
{
	width = w;
	height = h;
	pos_x = pos_y = pos_z = 0;
	rot = 0;
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

void gameObject::GetRotation(float& r)
{
	r = rot;
	return;
}

void gameObject::SetPosition(float x, float y, float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	return;
}

void gameObject::SetRotation(float r)
{
	rot = r;
	if (rot > 360)
		rot = 0;
	return;
}

void gameObject::AdjustPosition(float x, float y, float z)
{
	pos_x += x;
	pos_y += y;
	pos_z += z;
}

void gameObject::AdjustRotation(float r)
{
	rot += r;
	if (rot + r > 360)
		rot = 0;
}