#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class ModelClass;
class D3DClass;

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
		PLAYER,
		PLAYER_BULLET,
		BOSS,
		BOSS_BULLET,
		INTERACTION,
		NO_COLLISION,
	};
	enum ObjectType
	{
		MOVEABLE,
		AUTOMOVE,
		UNMOVABLE
	};

	gameObject(string, CollisionChannel, ColliderType=COLLIDER_BOX);
	gameObject(string, D3DXVECTOR3, CollisionChannel, ColliderType = COLLIDER_BOX);
	~gameObject();

	void InitializeObject();
	void PrintVector3(D3DXVECTOR3);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();
	D3DXVECTOR3 GetRotationAfter();
	D3DXVECTOR3 GetCollSize();

	void SetPosition(D3DXVECTOR3 z);
	void SetRotation(D3DXVECTOR3 z);
	void SetScale(D3DXVECTOR3 z);
	void SetRotationAfter(D3DXVECTOR3 z);
	void SetCollSize(D3DXVECTOR3 z);

	void AdjustPosition(D3DXVECTOR3);
	void AdjustRotation(D3DXVECTOR3);
	void AdjustScale(D3DXVECTOR3);

	void GetWorldMatrix(D3DXMATRIX&);

	virtual bool CheckDestroy() = 0;

	string GetName();
	ColliderType GetColliderType();
	ModelClass* GetModel();

	CollisionChannel channel;
	ObjectType objType;
	float sphere_collSize;
	float w;
	int scene;

protected:
	string name;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rot_after;
	D3DXVECTOR3 box_collSize;

	ModelClass* m_model;

	ColliderType collider;
	D3DClass* device;
};

#endif