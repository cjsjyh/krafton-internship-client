#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "gameObject.h"
#include "modelclass.h"
#include <d3dx10math.h>
#include <string>

class projectileclass : public gameObject
{
public:
	projectileclass(string, ModelClass*, ColliderType, D3DXVECTOR3, float);
	~projectileclass();
protected:

};

#endif

