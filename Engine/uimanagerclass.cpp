#include "stdafx.h"

#include "UIinfo.h"
#include "bitmapclass.h"
#include "imagedecoderclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "gameManager.h"
#include "bossclass.h"
#include "playerclass.h"
#include "textureshaderclass.h"
#include "textclass.h"

#include "socketInterface.h"
#include "timerclass.h"

#include "uimanagerclass.h"

uimanagerclass::uimanagerclass(vector<UIinfo*> _input, D3DClass* _device, HWND _hwnd, D3DXMATRIX _baseViewMatrix)
{
	m_TextureShader = 0;
	m_Text = 0;
	m_ImageDecoder = 0;
	player[0] = 0;
	player[1] = 0;
	boss = 0;
	GM = 0;
	camera = 0;
	blindAlpha = 1;
	timer = 0;
	startScreenOn = true;

	device = _device;
	parameters = _input;
	hwnd = _hwnd;
	baseViewMatrix = _baseViewMatrix;
}

uimanagerclass::~uimanagerclass()
{

}


void uimanagerclass::SetValues(int _screenWidth, int _screenHeight, CameraClass* _camera, gameManager* _GM)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	camera = _camera;
	GM = _GM;

	Initialize();
	InitializeUI();
}

bool uimanagerclass::Initialize()
{
	bool result;

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	result = m_TextureShader->Initialize(device->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}



	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	result = m_Text->Initialize(device->GetDevice(), device->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

}

bool uimanagerclass::InitializeUI()
{
	bool result;
	int width, height;
	UIinfo* uiinfo;
	m_ImageDecoder = new imagedecoderclass;

	for (int i = 0; i <parameters.size(); i++)
	{
		BitmapClass* temp = new BitmapClass;
		if (!temp)
			return false;
		//get file resolution
		uiinfo = parameters[i];
		m_ImageDecoder->GetImageSize(stdafx::StringToWchar(uiinfo->filename), width, height);
		if (uiinfo->size_x == 0 || uiinfo->size_y == 0)
		{
			uiinfo->size_x = width;
			uiinfo->size_y = height;
		}

		// Initialize the bitmap object.
		result = temp->Initialize(device->GetDevice(), screenWidth, screenHeight, stdafx::StringToWchar(uiinfo->filename), uiinfo->size_x, uiinfo->size_y);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
		m_UI.push_back(temp);
	}

	//FADE IN FADE OUT SCREEN
	BitmapClass* temp = new BitmapClass;
	uiinfo = new UIinfo;
	uiinfo->filename = "./data/UI/blackscreen.png";
	uiinfo->size_x = 2500;
	uiinfo->size_y = 2500;
	uiinfo->pos_x = uiinfo->pos_y = -500;
	uiinfo->uiname = "blackscreen";
	// Initialize the bitmap object.
	result = temp->Initialize(device->GetDevice(), screenWidth, screenHeight, stdafx::StringToWchar(uiinfo->filename), uiinfo->size_x, uiinfo->size_y);
	if (!result)
	{
		MessageBox(hwnd, L"!!Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	m_UI.push_back(temp);
	parameters.push_back(uiinfo);
	ScreenFade(1,-1,60);
	return true;
}

void uimanagerclass::ScreenFade(float startAlpha, float amount, int duration)
{
	blindAlpha = startAlpha;
	if (timer)
	{
		delete timer;
		timer = 0;
	}
	
	timer = new timerclass();
	timer->SetTimerGradualChange(&blindAlpha, amount, duration);
}

void uimanagerclass::ReplaceUI(string itemName, string filename)
{
	int width, height, index;
	UIinfo* uiinfo = 0;

	for (auto iter = parameters.begin(); iter != parameters.end(); ++iter)
	{
		if ((*iter)->uiname == itemName)
		{
			(*iter)->filename = "./data/UI/" + filename;
			uiinfo = *iter;
			index = iter - parameters.begin();
		}
	}
	BitmapClass* temp = new BitmapClass;
	temp->Initialize(device->GetDevice(), screenWidth, screenHeight, stdafx::StringToWchar(uiinfo->filename), uiinfo->size_x, uiinfo->size_y);

	m_UI[index]->Shutdown();
	m_UI[index] = 0;
	m_UI[index] = temp;
}

bool uimanagerclass::Render()
{
	bool result;
	D3DXMATRIX orthoMatrix;
	D3DXMATRIX worldMatrix;
	
	if (timer->Frame() && blindAlpha >= 1)
		ScreenFade(1, -1, 30);


	D3DXMatrixIdentity(&worldMatrix);
	device->GetOrthoMatrix(orthoMatrix);

	//SetUI(mouseX, mouseY, fps, cpu);
	/*result = m_Text->Render(device->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}*/

	if (player[0] == 0)
		player[0] = (playerclass*)(GM->GetGameObjectByTag("player"+to_string(socketInterface::playerId)));
	if (player[1] == 0)
		player[1] = (playerclass*)(GM->GetGameObjectByTag("player"+to_string(abs(socketInterface::playerId-1))));
	if (boss == 0)
		boss = (bossclass*)(GM->GetGameObject("boss"));

	RenderUI(m_UI, parameters, "");

	//Set player item images
	/*
	if (player->GetPlayerItemSize() > m_ItemUI.size())
	{
		int width, height;

		for (int i = m_ItemUI.size(); i < player->GetPlayerItemSize(); i++)
		{
			UIinfo* uiinfo = new UIinfo;
			BitmapClass* temp = new BitmapClass;

			if (!temp)
				return false;
			
			uiinfo->filename = "./data/UI/icon_" + player->GetPlayerItem(i) + ".png";
			//item bar x:10, y: 170
			uiinfo->pos_x = 30;
			uiinfo->pos_y = 680 - i * 70;
			uiinfo->size_x = uiinfo->size_y = 0;
			itemParameters.push_back(uiinfo);

			//get file resolution
			m_ImageDecoder->GetImageSize(stdafx::StringToWchar(uiinfo->filename), width, height);
			if (uiinfo->size_x == 0 || uiinfo->size_y == 0)
			{
				uiinfo->size_x = width;
				uiinfo->size_y = height;
			}

			// Initialize the bitmap object.
			result = temp->Initialize(device->GetDevice(), screenWidth, screenHeight, stdafx::StringToWchar(uiinfo->filename), uiinfo->size_x, uiinfo->size_y);
			m_ItemUI.push_back(temp);
		}
	}
	*/
	RenderUI(m_ItemUI, itemParameters, "");

	return true;
}

void uimanagerclass::RenderUI(vector<BitmapClass*> UIComp, vector<UIinfo*> UIparam, string fname)
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;
	bool result;

	D3DXMatrixIdentity(&worldMatrix);
	device->GetOrthoMatrix(orthoMatrix);

	for (int i = 0; i < UIComp.size(); i++)
	{
		if (UIparam[i]->toShow)
		{
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&worldMatrix);
			int x = UIparam[i]->pos_x;
			int y = UIparam[i]->pos_y;

			if (UIparam[i]->uiname == "BOSS_HPBAR_FRONT")
			{
				float bossHp = boss->GetHpPercent();
				D3DXMatrixScaling(&worldMatrix, bossHp, 1, 1);
			}
			else if (UIparam[i]->uiname == "PLAYER_HPBAR_FRONT")
			{
				float playerHp = player[0]->GetHpPercent();
				D3DXMatrixScaling(&worldMatrix, playerHp, 1, 1);
			}
			else if (UIparam[i]->uiname == "PLAYER2_HPBAR_FRONT")
			{
				float playerHp = player[1]->GetHpPercent();
				D3DXMatrixScaling(&worldMatrix, playerHp, 1, 1);
			}
			else if (UIparam[i]->uiname == "PLAYER_ULTI_FRONT")
			{
				float playerUlti = player[socketInterface::playerId]->GetUltiPercent();
				D3DXMatrixScaling(&worldMatrix, playerUlti, 1, 1);
			}

			result = UIComp[i]->Render(device->GetDeviceContext(), 0, 0);

			D3DXMatrixTranslation(&temp, -screenWidth / 2, screenHeight / 2, 0);
			worldMatrix *= temp;
			D3DXMatrixTranslation(&temp, x, -y, 0);
			worldMatrix *= temp;

			// Render the bitmap with the texture shader.
			if (UIparam[i]->uiname != "blackscreen")
				result = m_TextureShader->Render(device->GetDeviceContext(), UIComp[i]->GetIndexCount(), worldMatrix,
					baseViewMatrix, orthoMatrix, UIComp[i]->GetTexture());
			else
				result = m_TextureShader->Render(device->GetDeviceContext(), UIComp[i]->GetIndexCount(), worldMatrix,
					baseViewMatrix, orthoMatrix, UIComp[i]->GetTexture(), D3DXVECTOR4(1, 1, 1, blindAlpha));
		}
	}
}

void uimanagerclass::ToggleStartScreen()
{
	for (auto iter = parameters.begin(); iter != parameters.end(); iter++)
	{
		if ((*iter)->uiname != "START_SCREEN")
			(*iter)->toShow = startScreenOn;
		else
			(*iter)->toShow = !startScreenOn;
	}
	startScreenOn = !startScreenOn;
}

void uimanagerclass::TurnUIOff(string name)
{
	for (auto iter = parameters.begin(); iter != parameters.end(); iter++)
	{
		if ((*iter)->uiname == name)
			(*iter)->toShow = false;
	}
}


bool uimanagerclass::SetUI(int mouseX, int mouseY, int fps, int cpu)
{
	bool result;
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, device->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//----------------
	//   CPU / FPS
	//----------------
	result = m_Text->SetFps(fps, device->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, device->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}