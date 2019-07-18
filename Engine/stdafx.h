#pragma once
#ifndef _STDAFX_H_
#define _STDAFX_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11_1.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <d3dx11async.h>
#include <d3dx10math.h>

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
using namespace DirectX;

#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <limits>
#include <string>
#include <fstream>
#include "iostream"
using namespace std;

#define KEY_NUM 7

class stdafx
{
public :
	static float GetDistance(D3DXVECTOR3, D3DXVECTOR3);
	static D3DXVECTOR3 normalizeVec3(D3DXVECTOR3 vec);
	static void PrintVector3(D3DXVECTOR3 vec);
};

#endif