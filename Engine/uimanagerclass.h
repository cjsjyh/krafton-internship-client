#pragma once
#ifndef _UIMANAGERCLASS_H_
#define _UIMANAGERCLASS_H_

#include "textfilereader.h"
class BitmapClass;
class imagedecoderclass;
class D3DClass;

class uimanagerclass
{
public:


	uimanagerclass(vector<textfilereader::UIinfo>, D3DClass*);
	bool InitializeUI();

	void UpdateUI();
	void UpdateBossHpUI(float);
	void SetValues(int, int, HWND);

	vector<BitmapClass*> m_UI;
	vector<textfilereader::UIinfo> parameters;

private:
	imagedecoderclass* m_ImageDecoder;
	D3DClass* device;

	int screenWidth, screenHeight;
	HWND hwnd;
};

#endif