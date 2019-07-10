#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "d3dclass.h"
#include "iostream"
#include "modelclass.h"

#include <d3dx10math.h>
#include <string>
#include <math.h>
using namespace std;

#define CAM_SPEED 3
#define PLAYER_SPEED 0.5

class gameObject
{
public:
	enum ColliderType
	{
		COLLIDER_BOX,
		COLLIDER_COMPLEX,
	};


	gameObject(string, ModelClass*, ColliderType);
	gameObject(string, ModelClass*, ColliderType, float, float, float);
	~gameObject();

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);
	void GetScale(float&, float&, float&);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void AdjustPosition(float, float, float);
	void AdjustRotation(float, float, float);
	void AdjustScale(float, float, float);

	void GetWorldMatrix(D3DXMATRIX&);
	void GetSize(float&, float&, float&);
	string GetName();
	ColliderType GetColliderType();

	ModelClass* GetModel();

	float collider_size;
protected:
	string name;
	float pos_x, pos_y, pos_z;
	float scale_x, scale_y, scale_z;
	float rot_x,rot_y,rot_z;
	float w, h, l;

	ModelClass* m_model;
	ColliderType collider;
};

#endif