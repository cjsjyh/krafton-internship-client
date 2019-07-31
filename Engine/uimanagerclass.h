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
class TextClass;

class timerclass;

class uimanagerclass
{
public:
	uimanagerclass(vector<UIinfo*>, D3DClass*, HWND, D3DXMATRIX);
	~uimanagerclass();
	bool Initialize();
	bool InitializeUI();

	bool Render();
	void SetValues(int, int, CameraClass*, gameManager*);
	void ReplaceUI(string, string);
	void ScreenFade(float, float, int);

	vector<BitmapClass*> m_UI;
	vector<UIinfo*> parameters;
	vector<BitmapClass*> m_ItemUI;
	vector<UIinfo*> itemParameters;

private:
	bool SetUI(int, int, int, int);
	void RenderUI(vector<BitmapClass*>, vector<UIinfo*>, string fname);

	TextureShaderClass* m_TextureShader;
	TextClass* m_Text;
	imagedecoderclass* m_ImageDecoder;
	D3DClass* device;
	CameraClass* camera;
	gameManager* GM;
	bossclass* boss;
	playerclass* player;
	timerclass* timer;

	int screenWidth, screenHeight;
	HWND hwnd;
	D3DXMATRIX baseViewMatrix;
	float blindAlpha;
};

#endif
