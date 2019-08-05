////////////////////////////////////////////////////////////////////////////////
// Filename: ApplicationClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "d3dclass.h"
#include "inputclass.h"
#include "modelclass.h"

#include "cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "uimanagerclass.h"

#include "gameObject.h"
#include "staticobjclass.h"
#include "playerclass.h"
#include "projectileclass.h"
#include "bossclass.h"

#include "itemmanagerclass.h"
#include "gameManager.h"
#include "textfilereader.h"

#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_filereader = 0;
	m_D3D = 0;
	
	m_LightShader = 0;
	m_Light = 0;
	m_UIM = 0;

	//player = 0;
	boss = 0;
	m_GM = 0;
	m_IM = 0;
	m_Camera = 0;
	floor = rewardfloor = 0;

	mouseX = mouseY = 0;
	frame = 0;
	screenW = screenH = 0;
	last_scene_change_frame = 0;
	SCENE_CHANGE_COOLTIME = 60;
	
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND _hwnd)
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
	result = m_filereader->ReadFile("./data/datasheet/player_parameter.csv");
	if (!result)
		return false;
		
	result = m_filereader->ReadFile("./data/datasheet/boss_parameter.csv");
	if (!result)
		return false;
	
	result = m_filereader->ReadUIFile("./data/datasheet/ui_parameter.csv");
	if (!result)
		return false;

	result = m_filereader->ReadItemFile("./data/datasheet/item_parameter.csv");
	if (!result)
		return false;

	// Set baseViewMatrix for UI
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));
	m_Camera->Render(D3DXVECTOR3(0, 0, 0));
	m_Camera->GetViewMatrix(baseViewMatrix);

	InitializeBasic();
	InitializeMap();
	InitializePlayerParameters();
	InitializeBossParameters();
	return true;
}

void ApplicationClass::InitializeBasic()
{
	// Initial camera setup
	m_Camera->SetPosition(D3DXVECTOR3(0, 30, -30));
	m_Camera->SetRotation(D3DXVECTOR3(45, 0, 0));

	// Create gameManager object
	m_GM = new gameManager(SCENE_COUNT);
	m_GM->scene = 0;

	m_IM = new itemmanagerclass();
	m_IM->SetParameter(m_filereader);

	m_UIM = new uimanagerclass(m_filereader->paramUI, m_D3D, hwnd, baseViewMatrix);
	m_UIM->SetValues(screenW, screenH, m_Camera, m_GM);

	last_scene_change_frame = 0;
	return;
}

void ApplicationClass::UninitializeBasic()
{
	delete m_GM;
	m_GM = 0;
	
	delete m_IM;
	m_IM = 0;
	
	delete m_UIM;
	m_UIM = 0;
	
	return;
}

void ApplicationClass::InitializePlayerParameters()
{		
	//TEMP//
	for (int i = 0; i < playerCount; i++)
	{
		playerclass* player = players[i];
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
	}
	return;
}

void ApplicationClass::InitializeBossParameters()
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

void ApplicationClass::InitializeMap() 
{
	floor = new staticobjclass("floor",m_D3D, gameObject::NO_COLLISION, gameObject::COLLIDER_BOX);
	((staticobjclass*)floor)->InitializeStatic3D();
	floor->SetScale(D3DXVECTOR3(40, 0.1, 40));
	floor->SetPosition(D3DXVECTOR3(0, -5, 0));
	floor->SetRotation(D3DXVECTOR3(0, 45, 0));
	m_GM->RegisterObjectToRender(floor);
	
	for (int i = 0; i < playerCount; i++)
	{
		playerclass* player = new playerclass(10, m_D3D);
		player->SetManager(m_GM, m_IM);
		m_GM->RegisterObjectToRender(player);
		m_IM->SetManagers(m_GM, m_UIM, m_D3D);
		players.push_back(player);
	}
	players[1]->SetPosition(D3DXVECTOR3(0, 0, -10));

	boss = new bossclass(30, 1, m_D3D, players[0]);
	boss->SetPosition(D3DXVECTOR3(0, 0, 20));
	boss->SetGameManager(m_GM);
	m_GM->RegisterObjectToRender(boss);

	midPoint = (players[0]->GetPosition() + boss->GetPosition()) / 2;

	return;
}

void ApplicationClass::UninitializeMap()
{

	if (players.size() != 0)
	{
		players.clear();
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

void ApplicationClass::InitializeRewardMap(vector<string> itemNames)
{
	m_GM->RegisterObjectToRender(players[0], 1);
	m_GM->RegisterObjectToRender(players[1], 1);

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
		m_GM->RegisterObjectToRender(temp, 1);
	}

	return;
}

void ApplicationClass::UninitializeRewardMap()
{
	for (int i = 0; i < m_GM->GetRenderObjectCount(1); i++)
	{
		if(m_GM->GetGameObject(i,1)->GetName() == "player")
			m_GM->UnregisterObjectToRender(m_GM->GetGameObject(i, 1), 1);
		else
			m_GM->RemoveObjectToRender(m_GM->GetGameObject(i, 1), 1);
	}
}

void ApplicationClass::Shutdown()
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

D3DXVECTOR3 ApplicationClass::GetDirectionMouse(int _mouseX, int _mouseY)
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
	D3DXMatrixTranslation(&temp, players[0]->GetPosition().x, players[0]->GetPosition().y, players[0]->GetPosition().z);
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

bool ApplicationClass::Frame(PlayerInfotemp playerInput)
{
	bool result;

	//mouseX = mouseX - screenW / 2;
	//mouseY = -(mouseY - screenH / 2);

	frame++;

	//Frame Action
	if (m_GM->scene == 0)
		boss->Frame(frame);
	//player->Frame(m_Input->keyInput, m_Input->mouseInput, GetDirectionMouse(mouseX, mouseY) ,frame);
	
	for(int i =0; i<players.size(); i++)
		players[i]->Frame(playerInput.keyInput, playerInput.mouseInput, GetDirectionMouse(playerInput.mouseX, playerInput.mouseY), frame);
	m_GM->Frame();
	SetCamera(m_GM->scene);
	
	//PLAYER DEAD
	if (players[0]->CheckDestroy() || boss->CheckDestroy())
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
	//  SCENE CHANGE
	//-------------------
	if (InputClass::IsKeyPressed(playerInput.keyInput, DIK_LSHIFT))
	{
		if (frame - last_scene_change_frame > SCENE_CHANGE_COOLTIME)
		{
			last_scene_change_frame = frame;
			if (m_GM->scene == 0)
			{
				m_UIM->ScreenFade(1, -1, 30);
				vector<string> itemNames;
				players[0]->SavePlayerPos(m_GM->scene);
				players[0]->SetPosition(D3DXVECTOR3(0, 0, 0));
				players[0]->SetDirection(1);
				
				itemNames = m_IM->ChooseItemFromPool(3,0);
				InitializeRewardMap(itemNames);
				m_GM->scene = 1;
			}
			else
			{
				m_UIM->ScreenFade(1, -1, 30);
				m_GM->scene = 0;
				players[0]->SetPosition(players[0]->GetSavedPlayerPos(m_GM->scene));

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

bool ApplicationClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX MatrixToFaceCamera;
	bool result;

	//-----------------------
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(players[0]->GetPosition());
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
		
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), temp->GetModel()->GetIndexCount(), worldMatrix,
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

	m_UIM->Render();

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	m_D3D->EndScene();

	return true;
}


void ApplicationClass::SetCamera(int scene)
{
	if (scene == 0)
	{
		m_Camera->SetViewPoint((players[0]->GetPosition() + boss->GetPosition()) / 2);
		float distance = stdafx::GetDistance(players[0]->GetPosition(), boss->GetPosition());
		m_Camera->Move(distance);
	}
	else if (scene == 1)
	{
		m_Camera->SetPosition(D3DXVECTOR3(0, 30, -30));
		m_Camera->SetViewPoint(D3DXVECTOR3(0, 0, 0));
	}
}