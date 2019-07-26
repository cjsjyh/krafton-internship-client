#pragma once
#ifndef _UIMANAGERCLASS_H_
#define _UIMANAGERCLASS_H_


class UIinfo;
class BitmapClass;
class imagedecoderclass;
class D3DClass;
class CameraClass;
class gameManager;
class bossclass;
class playerclass;
class TextureShaderClass;


class uimanagerclass
{
public:
	uimanagerclass(vector<UIinfo*>, D3DClass*, HWND);
	~uimanagerclass();
	bool InitializeUI();

	bool Render(D3DXMATRIX);
	void SetValues(int, int, CameraClass*, gameManager*);

	vector<BitmapClass*> m_UI;
	vector<UIinfo*> parameters;

private:
	TextureShaderClass* m_TextureShader;
	imagedecoderclass* m_ImageDecoder;
	D3DClass* device;
	CameraClass* camera;
	gameManager* GM;
	bossclass* boss;
	playerclass* player;

	int screenWidth, screenHeight;
	HWND hwnd;
};

#endif