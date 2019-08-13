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

//WIC
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include <d3d11_1.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <d3dx11async.h>
#include <d3dx10math.h>

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
using namespace DirectX;

#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <math.h>
#include <limits>
#include <string>
#include <fstream>
#include "iostream"
using namespace std;



#define TO_RADIAN 0.0174532925f
#define MAX_PLAYER_COUNT 2
#define KEY_NUM 8

typedef struct BossPatternFile {
	int id;
	int phase;
	int bullet_type;
	int bullet_slow_frame;
	float bullet_speed;
	int dirCount;
	int angleBetw;
	int life;
	int repeat;
	int delay;
	int rotAngle;
}BossPatternFile;

class stdafx
{
public :
	static float GetDistance(D3DXVECTOR3, D3DXVECTOR3);
	static D3DXVECTOR3 normalizeVec3(D3DXVECTOR3 vec);
	static void PrintVector3(D3DXVECTOR3 vec);
	static WCHAR* StringToWchar(string);
};

#endif