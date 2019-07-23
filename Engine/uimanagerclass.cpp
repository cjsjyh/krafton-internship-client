#include "stdafx.h"

//#include "textfilereader.h"
#include "bitmapclass.h"
#include "imagedecoderclass.h"
#include "d3dclass.h"

#include "uimanagerclass.h"

uimanagerclass::uimanagerclass(vector<textfilereader::UIinfo> _input, D3DClass* _device)
{
	m_ImageDecoder = 0;
	device = _device;
	parameters = _input;
}

void uimanagerclass::SetValues(int _screenWidth, int _screenHeight, HWND _hwnd)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	hwnd = _hwnd;
}

bool uimanagerclass::InitializeUI()
{
	bool result;
	int width, height;
	textfilereader::UIinfo uiinfo;

	m_ImageDecoder = new imagedecoderclass;

	for (int i = 0; i <parameters.size(); i++)
	{
		BitmapClass* temp = new BitmapClass;
		if (!temp)
			return false;
		//get file resolution
		uiinfo = parameters[i];

		m_ImageDecoder->GetImageSize(stdafx::StringToWchar(uiinfo.filename), width, height);
		if (uiinfo.size_x == 0 || uiinfo.size_y == 0)
		{
			uiinfo.size_x = width;
			uiinfo.size_y = height;
		}

		// Initialize the bitmap object.
		result = temp->Initialize(device->GetDevice(), screenWidth, screenHeight, stdafx::StringToWchar(uiinfo.filename), uiinfo.size_x, uiinfo.size_y);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
		m_UI.push_back(temp);

	}
	return true;
}
