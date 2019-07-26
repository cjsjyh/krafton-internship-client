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

#include "uimanagerclass.h"

uimanagerclass::uimanagerclass(vector<UIinfo*> _input, D3DClass* _device, HWND _hwnd, D3DXMATRIX _baseViewMatrix)
{
	m_TextureShader = 0;
	m_Text = 0;
	m_ImageDecoder = 0;
	player = 0;
	boss = 0;
	GM = 0;
	camera = 0;

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
	return true;
}

bool uimanagerclass::Render(int mouseX, int mouseY, int fps, int cpu)
{
	bool result;
	D3DXMATRIX orthoMatrix;
	D3DXMATRIX worldMatrix;
	
	D3DXMatrixIdentity(&worldMatrix);
	device->GetOrthoMatrix(orthoMatrix);

	SetUI(mouseX, mouseY, fps, cpu);
	result = m_Text->Render(device->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	if (player == 0)
		player = (playerclass*)GM->GetGameObject("player");
	if (boss == 0)
		boss = (bossclass*)GM->GetGameObject("boss");
		
	for (int i = 0; i < m_UI.size(); i++)
	{
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&worldMatrix);
		int x = parameters[i]->pos_x;
		int y = parameters[i]->pos_y;


		if (parameters[i]->uiname == "BOSS_HPBAR_FRONT")
		{
			float bossHp = boss->GetHpPercent();
			D3DXMatrixScaling(&worldMatrix, 1 - bossHp, 1, 1);
		}
		else if (parameters[i]->uiname == "PLAYER_HPBAR_FRONT")
		{
			float playerHp = player->GetHpPercent();
			D3DXMatrixScaling(&worldMatrix, 1 - playerHp, 1, 1);
		}

		result = m_UI[i]->Render(device->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}
		D3DXMatrixTranslation(&temp, -screenWidth / 2, screenHeight / 2, 0);
		worldMatrix *= temp;
		D3DXMatrixTranslation(&temp, x, -y, 0);
		worldMatrix *= temp;

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(device->GetDeviceContext(), m_UI[i]->GetIndexCount(), worldMatrix,
			baseViewMatrix, orthoMatrix, m_UI[i]->GetTexture());
		if (!result)
		{
			return false;
		}
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