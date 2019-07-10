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
	gameObject(string, ModelClass*, ColliderType, D3DXVECTOR3);
	~gameObject();

	void GetPosition(D3DXVECTOR3&);
	void GetRotation(D3DXVECTOR3&);
	void GetScale(D3DXVECTOR3&);

	void SetPosition(D3DXVECTOR3 z);
	void SetRotation(D3DXVECTOR3 z);
	void SetScale(D3DXVECTOR3 z);

	void AdjustPosition(D3DXVECTOR3);
	void AdjustRotation(D3DXVECTOR3);
	void AdjustScale(D3DXVECTOR3);

	void GetWorldMatrix(D3DXMATRIX&);
	void GetSize(D3DXVECTOR3&);
	string GetName();
	ColliderType GetColliderType();

	ModelClass* GetModel();

	float sphere_collSize;
protected:
	string name;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 box_collSize;

	ModelClass* m_model;
	ColliderType collider;
};

#endif