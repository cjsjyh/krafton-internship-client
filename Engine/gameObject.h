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

	gameObject(string, ModelClass*, ColliderType, CollisionChannel=HIT_PLAYER, ObjectType=AUTOMOVE);
	gameObject(string, ModelClass*, ColliderType, D3DXVECTOR3, CollisionChannel=HIT_PLAYER, ObjectType = AUTOMOVE);
	~gameObject();

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
	virtual void Move() = 0;

	string GetName();
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
	
};

#endif