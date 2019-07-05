#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3dx10math.h>
#include "iostream"

class gameObject
{
public:
	gameObject();
	gameObject(float, float);
	~gameObject();

	void GetPosition(float&, float&, float&);
	void GetRotation(float&);

	void gameObject::SetPosition(float x, float y, float z);
	void gameObject::SetRotation(float r);

	void AdjustPosition(float, float, float);
	void AdjustRotation(float);

private:
	float pos_x, pos_y, pos_z;
	float rot;
	float height, width;
};

#endif