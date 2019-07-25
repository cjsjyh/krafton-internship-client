////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "d3dclass.h"
#include "inputclass.h"
#include "modelclass.h"

#include "cameraclass.h"
#include "textureshaderclass.h"
#include "textclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "uimanagerclass.h"
#include "bitmapclass.h"

#include "gameObject.h"
#include "staticobjclass.h"
#include "playerclass.h"
#include "projectileclass.h"
#include "bossclass.h"

#include "itemmanagerclass.h"
#include "gameManager.h"
#include "textfilereader.h"

#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	
	m_Text = 0;
	m_UIManager = 0;

	m_TextureShader = 0;
	m_GM = 0;

	frame = 0;
	lastLeftClick = 0;
	m_filereader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND _hwnd)
{
	bool result;
	
	screenW = screenWidth;
	screenH = screenHeight;
	hwnd = _hwnd;

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.

	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));
	m_Camera->Render(D3DXVECTOR3(0, 0, 0));
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	//------------
	//   text
	//------------
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.	
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);

	m_filereader = new textfilereader;
	result = m_filereader->ReadFile("../Engine/data/datasheet/player_parameter.csv");
	if (!result)
		return false;
		
	result = m_filereader->ReadFile("../Engine/data/datasheet/boss_parameter.csv");
	if (!result)
		return false;
	
	result = m_filereader->ReadUIFile("../Engine/data/datasheet/ui_parameter.csv");
	if (!result)
		return false;

	result = m_filereader->ReadItemFile("../Engine/data/datasheet/item_parameter.csv");
	if (!result)
		return false;

	InitializeBasic();
	InitializeMap();
	InitializePlayerParameters();
	InitializeBossParameters();
	return true;
}



void GraphicsClass::InitializeBasic()
{
	//initial camera setup
	m_Camera->SetPosition(D3DXVECTOR3(0, 30, -30));
	m_Camera->SetRotation(D3DXVECTOR3(45, 0, 0));

	// Create gameManager object
	m_GM = new gameManager(SCENE_COUNT);
	m_GM->scene = 0;

	m_IM = new itemmanagerclass();
	m_IM->SetParameter(m_filereader);

	m_UIManager = new uimanagerclass(m_filereader->paramUI, m_D3D);
	m_UIManager->SetValues(screenW, screenH, hwnd);
	m_UIManager->InitializeUI();

	
	sceneChangeFrame = 0;
	return;
}

void GraphicsClass::UninitializeBasic()
{
	delete m_GM;
	m_GM = 0;

	return;
}

void GraphicsClass::InitializePlayerParameters()
{		
	player->PLAYER_SPEED = m_filereader->paramFloat.find("PLAYER_SPEED")->second;
	player->PLAYER_DASH_SPEED = m_filereader->paramFloat.find("PLAYER_DASH_SPEED")->second;
	player->PLAYER_DASH_FRAME = m_filereader->paramInt.find("PLAYER_DASH_FRAME")->second;
	player->PLAYER_DASH_PAUSE_FRAME = m_filereader->paramInt.find("PLAYER_DASH_PAUSE_FRAME")->second;
	
	player->PLAYER_BULLET_DAMAGE = m_filereader->paramInt.find("PLAYER_BULLET_DAMAGE")->second;
	player->PLAYER_BULLET_SPEED = m_filereader->paramFloat.find("PLAYER_BULLET_SPEED")->second;
	player->PLAYER_BULLET_DISTANCE = m_filereader->paramInt.find("PLAYER_BULLET_DISTANCE")->second;
	player->PLAYER_BULLET_DELAY = m_filereader->paramInt.find("PLAYER_BULLET_DELAY")->second;
	
	player->SetHp(m_filereader->paramInt.find("PLAYER_HP")->second);
	player->PLAYER_INTERACTION_RANGE = m_filereader->paramFloat.find("PLAYER_INTERACTION_RANGE")->second;
	
	D3DXVECTOR3 playerSize;
	playerSize.x = m_filereader->paramFloat.find("PLAYER_SIZE_X")->second;
	playerSize.y = m_filereader->paramFloat.find("PLAYER_SIZE_Y")->second;
	playerSize.z = m_filereader->paramFloat.find("PLAYER_SIZE_Z")->second;
	player->SetScale(playerSize);

	D3DXVECTOR3 playerCollSize;
	playerCollSize.x = m_filereader->paramFloat.find("PLAYER_COLLIDER_SIZE_X")->second;
	playerCollSize.y = m_filereader->paramFloat.find("PLAYER_COLLIDER_SIZE_Y")->second;
	playerCollSize.z = m_filereader->paramFloat.find("PLAYER_COLLIDER_SIZE_Z")->second;
	player->SetCollSize(playerCollSize);

	return;
}

void GraphicsClass::InitializeBossParameters()
{
	D3DXVECTOR3 bossSize;
	bossSize.x = m_filereader->paramFloat.find("BOSS_PHASE1_SIZE_X")->second;
	bossSize.y = m_filereader->paramFloat.find("BOSS_PHASE1_SIZE_Y")->second;
	bossSize.z = m_filereader->paramFloat.find("BOSS_PHASE1_SIZE_Z")->second;
	boss->SetScale(bossSize);
	boss->BOSS_SIZE[0] = bossSize;

	bossSize.x = m_filereader->paramFloat.find("BOSS_PHASE2_SIZE_X")->second;
	bossSize.y = m_filereader->paramFloat.find("BOSS_PHASE2_SIZE_Y")->second;
	bossSize.z = m_filereader->paramFloat.find("BOSS_PHASE2_SIZE_Z")->second;
	boss->BOSS_SIZE[1] = bossSize;

	bossSize.x = m_filereader->paramFloat.find("BOSS_PHASE3_SIZE_X")->second;
	bossSize.y = m_filereader->paramFloat.find("BOSS_PHASE3_SIZE_Y")->second;
	bossSize.z = m_filereader->paramFloat.find("BOSS_PHASE3_SIZE_Z")->second;
	boss->BOSS_SIZE[2] = bossSize;

	D3DXVECTOR3 bossCollSize;
	bossCollSize.x = m_filereader->paramFloat.find("BOSS_COLLIDER_SIZE_X")->second;
	bossCollSize.y = m_filereader->paramFloat.find("BOSS_COLLIDER_SIZE_Y")->second;
	bossCollSize.z = m_filereader->paramFloat.find("BOSS_COLLIDER_SIZE_Z")->second;
	boss->SetCollSize(bossCollSize);

	boss->SetHp(m_filereader->paramInt.find("BOSS_HP")->second);
	boss->BOSS_PHASE3_HP = m_filereader->paramFloat.find("BOSS_PHASE3_HP")->second;
	boss->BOSS_PHASE2_HP = m_filereader->paramFloat.find("BOSS_PHASE2_HP")->second;

	return;
}

void GraphicsClass::InitializeMap() 
{
	floor = new staticobjclass("floor",m_D3D, gameObject::NO_COLLISION, gameObject::COLLIDER_BOX);
	((staticobjclass*)floor)->InitializeStatic3D();
	floor->SetScale(D3DXVECTOR3(40, 0.1, 40));
	floor->SetPosition(D3DXVECTOR3(0, -5, 0));
	floor->SetRotation(D3DXVECTOR3(0, 45, 0));
	m_GM->RegisterObjectToRender(floor);
	
	
	player = new playerclass(10, m_D3D);
	player->SetManager(m_GM,m_IM);
	m_GM->RegisterObjectToRender(player);
	m_IM->SetPlayer(player);

	boss = new bossclass(30, 1, m_D3D, player);
	boss->SetPosition(D3DXVECTOR3(0, 0, 20));
	boss->SetGameManager(m_GM);
	m_GM->RegisterObjectToRender(boss);

	midPoint = (player->GetPosition() + boss->GetPosition()) / 2;

	return;
}

void GraphicsClass::UninitializeMap()
{
	if (player)
	{
		delete player;
		player = 0;
	}
	if (boss)
	{
		delete boss;
		boss = 0;
	}
	if (floor)
	{
		delete floor;
		floor = 0;
	}
	
	return;
}

void GraphicsClass::InitializeRewardMap(vector<string> itemNames)
{
	m_GM->RegisterObjectToRender(player, 1);

	rewardfloor = new staticobjclass("floor", m_D3D, gameObject::NO_COLLISION,gameObject::COLLIDER_BOX);
	((staticobjclass*)rewardfloor)->InitializeStatic3D();
	rewardfloor->SetScale(D3DXVECTOR3(40, 0.1, 40));
	rewardfloor->SetPosition(D3DXVECTOR3(0, -5, 0));
	rewardfloor->SetRotation(D3DXVECTOR3(0, 45, 0));
	m_GM->RegisterObjectToRender(rewardfloor, 1);

	//add box
	for (int i = 0; i < itemNames.size(); i++)
	{
		gameObject* temp;
		temp = new staticobjclass(itemNames[i], m_D3D, gameObject::INTERACTION, gameObject::COLLIDER_BOX);
		((staticobjclass*)temp)->InitializeStatic2DItem("shop_");
		temp->SetScale(D3DXVECTOR3(2, 2, 2));
		temp->SetPosition(D3DXVECTOR3(-10 + 10*i, 0, 10));
		temp->SetRotationAfter(D3DXVECTOR3(0, 45, 0));
		//temp->SetRotation(D3DXVECTOR3(0, 45, 0));
		m_GM->RegisterObjectToRender(temp, 1);
	}

	return;
}

void GraphicsClass::UninitializeRewardMap()
{
	for (int i = 0; i < m_GM->GetRenderObjectCount(1); i++)
	{
		if(m_GM->GetGameObject(i,1)->GetName() == "player")
			m_GM->UnregisterObjectToRender(m_GM->GetGameObject(i, 1), 1);
		else
			m_GM->RemoveObjectToRender(m_GM->GetGameObject(i, 1), 1);
	}
}

void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}


	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_GM)
	{
		delete m_GM;
		m_GM = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

D3DXVECTOR3 GraphicsClass::GetDirectionMouse(int _mouseX, int _mouseY)
{
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection, projectedPt;
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, temp;

	float pointX = ((2.0f * _mouseX) / screenW) - 1;
	float pointY = (((2.0f * _mouseY) / screenH) - 1) * -1;

	m_D3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	m_Camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	origin = m_Camera->GetPosition();

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&temp, player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &temp);
	D3DXMatrixInverse(&worldMatrix, NULL, &worldMatrix);

	D3DXVec3TransformCoord(&rayOrigin, &origin, &worldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &worldMatrix);
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	projectedPt = D3DXVECTOR3(0, 0, 0);
	float k = -(rayOrigin.y / rayDirection.y);
	projectedPt.x = rayOrigin.x + rayDirection.x * k;
	projectedPt.z = rayOrigin.z + rayDirection.z * k;

	return stdafx::normalizeVec3(projectedPt);
}

bool GraphicsClass::Frame(int _mouseX, int _mouseY, bool* mousePress, int* key, int fps, int cpu)
{
	
	bool result;
	mouseX = _mouseX - screenW / 2;
	mouseY = -(_mouseY - screenH / 2);

	frame++;
	//UI
	if (!SetUI(mouseX, mouseY, fps, cpu))
		return false;

	//CAN CLICK AGAIN!
	if (frame - lastLeftClick > MOUSE_FRAME_RATE)
		lastLeftClick = 0;

	//-------------------
	//   Frame Action
	//-------------------
	
	
	if (m_GM->scene == 0)
	{
		//BOSS
		vector<projectileclass*> bossBullet = boss->Frame(frame);
		for (unsigned int i = 0; i < bossBullet.size(); i++)
			m_GM->RegisterObjectToRender(bossBullet[i]);

		//CAMERA
		m_Camera->SetViewPoint((player->GetPosition() + boss->GetPosition()) / 2);
		float distance = stdafx::GetDistance(player->GetPosition(), boss->GetPosition());
		m_Camera->Move(distance);
	}
	else if (m_GM->scene == 1)
	{
		//CAMERA
		m_Camera->SetPosition(D3DXVECTOR3(0, 30, -30));
		m_Camera->SetViewPoint(D3DXVECTOR3(0, 0, 0));
	}


	player->Frame(key, frame);
	m_GM->Frame();

	//Camera
	
	//PLAYER DEAD
	if (player->CheckDestroy())
	{
		UninitializeMap();
		UninitializeBasic();

		InitializeBasic();
		InitializeMap();
		InitializePlayerParameters();
		InitializeBossParameters();
		return true;
	}
	
	//-------------------
	//  Input Handler
	//-------------------
	if (InputClass::LeftMouseClicked(mousePress))
	{    
		if (!lastLeftClick)
		{
			m_GM->RegisterObjectToRender(player->Fire(GetDirectionMouse(_mouseX,_mouseY)),m_GM->scene);
			lastLeftClick = frame;
		}
	}
	
	if (InputClass::IsKeyPressed(key, 'T'))
	{
		if (frame - sceneChangeFrame > 60)
		{
			sceneChangeFrame = frame;
			if (m_GM->scene == 0)
			{
				vector<string> itemNames;

				player->SavePlayerPos(m_GM->scene);
				player->SetPosition(D3DXVECTOR3(0, 0, 0));
				player->SetDirection(1);
				
				
				
				itemNames = m_IM->ChooseItemFromPool(3,0);
				for (auto iter = itemNames.begin(); iter != itemNames.end(); iter++)
					cout << *iter << endl;
				InitializeRewardMap(itemNames);
				
				m_GM->scene = 1;
			}
			else
			{
				m_GM->scene = 0;
				player->SetPosition(player->GetSavedPlayerPos(m_GM->scene));

				UninitializeRewardMap();
			}
		}
	}
	
	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}
	
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX MatrixToFaceCamera;
	bool result;

	//-----------------------
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(player->GetPosition());
	m_Camera->GetBillBoardMatrix(MatrixToFaceCamera);
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	//-----------------
	// Transformation
	//-----------------
	m_D3D->TurnOnAlphaBlending();

	//render floor first
	m_GM->AlphaSort(m_Camera->GetPosition());
	int size = m_GM->GetRenderObjectCount();
	for (int i = 0; i < size; i++)
	{
		gameObject* temp = m_GM->GetGameObject(i);
		temp->GetModel()->Render(m_D3D->GetDeviceContext());
		temp->GetWorldMatrix(worldMatrix);
		if (temp->GetName() != "floor")
			worldMatrix = MatrixToFaceCamera *  worldMatrix;
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), temp->GetModel()->GetIndexCount(),worldMatrix,
									viewMatrix, projectionMatrix, temp->GetModel()->GetTexture(), m_Light->GetDirection(),
									m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}


	//---------------------
	//   TEXT
	//---------------------

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	D3DXMatrixIdentity(&worldMatrix);
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	for (int i=0;i < m_UIManager->m_UI.size(); i++)
	{
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&worldMatrix);
		int x = m_UIManager->parameters[i].pos_x;
		int y = m_UIManager->parameters[i].pos_y;
		
		
		if (m_UIManager->parameters[i].uiname == "BOSS_HPBAR_FRONT")
		{
			float bossHp = boss->GetHpPercent();
			D3DXMatrixScaling(&worldMatrix, 1-bossHp, 1, 1);
		}
		else if (m_UIManager->parameters[i].uiname == "PLAYER_HPBAR_FRONT")
		{
			float playerHp = player->GetHpPercent();
			D3DXMatrixScaling(&worldMatrix, 1-playerHp, 1, 1);
		}
		
		result = m_UIManager->m_UI[i]->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}
		D3DXMatrixTranslation(&temp, -screenW / 2, screenH / 2, 0);
		worldMatrix *= temp;
		D3DXMatrixTranslation(&temp, x, -y, 0);
		worldMatrix *= temp;

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_UIManager->m_UI[i]->GetIndexCount(), worldMatrix, 
										baseViewMatrix, orthoMatrix, m_UIManager->m_UI[i]->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::SetUI(int mouseX, int mouseY, int fps, int cpu)
{
	bool result;
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//----------------
	//   CPU / FPS
	//----------------
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}
