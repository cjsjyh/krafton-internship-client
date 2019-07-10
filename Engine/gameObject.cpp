#include "gameObject.h"


gameObject::gameObject(string objname, ModelClass* model, ColliderType col, CollisionChannel _channel)
{
	pos.x = pos.y = pos.z = 0;
	scale.x = scale.y = scale.z = 1;
	rot.x = rot.y = rot.z = 0;
	box_collSize.x = box_collSize.y = box_collSize.z = 1;
	sphere_collSize = 0.5;

	name = objname;
	m_model = model;
	collider = col;
	channel = _channel;
}

gameObject::gameObject(string objname, ModelClass* model, ColliderType col, D3DXVECTOR3 _pos, CollisionChannel _channel)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	pos.z = _pos.z;

	scale.x = scale.y = scale.z = 1;
	rot.x = rot.y = rot.z = 0;
	box_collSize.x = box_collSize.y = box_collSize.z = 1;

	name = objname;
	m_model = model;
	collider = col;
	channel = _channel;
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

void gameObject::GetSize(D3DXVECTOR3& _size)
{
	_size.x = box_collSize.x;
	_size.y = box_collSize.y;
	_size.z = box_collSize.z;
	return;
}

void gameObject::GetPosition(D3DXVECTOR3& _pos)
{
	_pos.x = pos.x;
	_pos.y = pos.y;
	_pos.z = pos.z;
	return;
}

void gameObject::GetRotation(D3DXVECTOR3& _rot)
{
	_rot.x = rot.x;
	_rot.y = rot.y;
	_rot.z = rot.z;

	return;
}

void gameObject::GetScale (D3DXVECTOR3& _scale)
{
	_scale.x = scale.x;
	_scale.y = scale.y;
	_scale.z = scale.z;

	return;
}

void gameObject::SetPosition(D3DXVECTOR3 _pos)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	pos.z = _pos.z;
	return;
}

void gameObject::SetRotation(D3DXVECTOR3 _rot)
{
	rot.x = _rot.x;
	while (rot.x > 360)
		rot.x = rot.x - 360;

	rot.y = _rot.y;
	while (rot.y > 360)
		rot.y = rot.y - 360;

	rot.z = _rot.z;
	while (rot.z > 360)
		rot.z = rot.z - 360;
	return;
}

void gameObject::SetScale(D3DXVECTOR3 _scale)
{
	//width
	scale.x = _scale.x;
	box_collSize.x *= _scale.x;

	//height
	scale.y = _scale.y;
	box_collSize.y *= _scale.y;

	//length
	scale.z = _scale.z;
	box_collSize.z *= _scale.z;
	
	sphere_collSize = min(min(_scale.x, _scale.y), _scale.z) * 0.75;
	return;
}

void gameObject::AdjustPosition(D3DXVECTOR3 _pos)
{
	pos.x += _pos.x;
	pos.y += _pos.y;
	pos.z += _pos.z;
	return;
}

void gameObject::AdjustRotation(D3DXVECTOR3 _rot)
{
	rot.x += _rot.x;
	while (rot.x > 360)
		rot.x = rot.x - 360;

	rot.y += _rot.y;
	while (rot.y > 360)
		rot.y = rot.y - 360;

	rot.z += _rot.z;
	while (rot.z > 360)
		rot.z = rot.z - 360;
	return;
}

void gameObject::AdjustScale(D3DXVECTOR3 _scale)
{
	scale.x += _scale.x;
	box_collSize.x += _scale.x * 2;

	scale.y += _scale.y;
	box_collSize.y += _scale.y * 2;

	scale.z += _scale.z;
	box_collSize.z += _scale.z * 2;
}

void gameObject::GetWorldMatrix(D3DXMATRIX& WorldMatrix)
{
	D3DXMATRIX temp;
	
	D3DXMatrixIdentity(&WorldMatrix);

	D3DXMatrixScaling(&temp, scale.x, scale.y, scale.z);
	WorldMatrix *= temp;

	D3DXMatrixRotationX(&temp, rot.x * 0.0174532925f);
	WorldMatrix *= temp;
	D3DXMatrixRotationY(&temp, rot.y * 0.0174532925f);
	WorldMatrix *= temp;
	D3DXMatrixRotationZ(&temp, rot.z * 0.0174532925f);
	WorldMatrix *= temp;

	D3DXMatrixTranslation(&temp, pos.x, pos.y, pos.z);
	WorldMatrix *= temp;
}