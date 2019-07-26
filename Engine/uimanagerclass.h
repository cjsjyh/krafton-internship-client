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

class uimanagerclass
{
public:
	uimanagerclass(vector<UIinfo*>, D3DClass*, HWND, D3DXMATRIX);
	~uimanagerclass();
	bool Initialize();
	bool InitializeUI();

	bool Render(int, int, int, int);
	void SetValues(int, int, CameraClass*, gameManager*);
	void ReplaceUI(string, string);

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

	int screenWidth, screenHeight;
	HWND hwnd;
	D3DXMATRIX baseViewMatrix;
};

#endif
