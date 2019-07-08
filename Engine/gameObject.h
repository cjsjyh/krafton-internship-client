#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3dx10math.h>
#include "iostream"

#define CAM_SPEED 3
#define PLAYER_SPEED 0.5

class gameObject
{
public:
	gameObject();
	gameObject(float, float, float);
	~gameObject();

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);
	void GetScale(float&, float&, float&);

	void gameObject::SetPosition(float x, float y, float z);
	void gameObject::SetRotation(float x, float y, float z);
	void gameObject::SetScale(float x, float y, float z);

	void AdjustPosition(float, float, float);
	void AdjustRotation(float, float, float);
	void AdjustScale(float, float, float);

private:
	float pos_x, pos_y, pos_z;
	float scale_x, scale_y, scale_z;
	float rot_x,rot_y,rot_z;
	float height, width;
};

#endif