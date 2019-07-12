#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "d3dclass.h"
#include "iostream"
#include "modelclass.h"

#include <d3dx10math.h>
#include <string>
#include <math.h>
#include <atlconv.h>
using namespace std;

class gameObject
{
public:
	enum ColliderType
	{
		COLLIDER_BOX,
		COLLIDER_COMPLEX,
	};
	enum CollisionChannel
	{
		HIT_PLAYER,
		HIT_BOSS,
		NO_COLLISION,
	};
	enum ObjectType
	{
		MOVEABLE,
		AUTOMOVE,
		UNMOVABLE
	};

	gameObject(string, CollisionChannel=HIT_PLAYER, ColliderType=COLLIDER_BOX);
	gameObject(string, D3DXVECTOR3, CollisionChannel=HIT_PLAYER, ColliderType = COLLIDER_BOX);
	~gameObject();

	void InitializeObject();
	void PrintVector3(D3DXVECTOR3);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
	D3DXVECTOR3 GetSize();

	void SetPosition(D3DXVECTOR3 z);
	void SetRotation(D3DXVECTOR3 z);
	void SetScale(D3DXVECTOR3 z);

	void AdjustPosition(D3DXVECTOR3);
	void AdjustRotation(D3DXVECTOR3);
	void AdjustScale(D3DXVECTOR3);

	void GetWorldMatrix(D3DXMATRIX&);

	virtual bool CheckDestroy() = 0;

	string GetName();
	WCHAR* StringToWchar(string);
	ColliderType GetColliderType();
	ModelClass* GetModel();

	float sphere_collSize;
	CollisionChannel channel;
	ObjectType objType;

protected:
	string name;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 box_collSize;

	ModelClass* m_model;
	ColliderType collider;
	D3DClass* device;
	bool toShow;
};

#endif