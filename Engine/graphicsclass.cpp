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

#include "gameObject.h"
#include "staticobjclass.h"
#include "playerclass.h"
#include "projectileclass.h"
#include "bossclass.h"

#include "gameManager.h"
#include "textfilereader.h"

#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	
	m_Text = 0;
	
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


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	screenW = screenWidth;
	screenH = screenHeight;

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

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));
	m_Camera->Render(D3DXVECTOR3(0,0,0));
	m_Camera->GetViewMatrix(baseViewMatrix);

	//initial camera setup
	m_Camera->SetPosition(D3DXVECTOR3(0, 30, -30));
	m_Camera->SetRotation(D3DXVECTOR3(45, 0, 0));

	//------------
	//   text
	//------------

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

	// Create gameManager object
	m_GM = new gameManager;
	if (!m_GM)
	{
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

	InitializeMap();
	InitializeParameters();

	return true;
}

void GraphicsClass::InitializeParameters()
{
	m_filereader = new textfilereader;
	bool result = m_filereader->ReadFile("../Engine/data/parameter.csv");
	
	if (!result)
		return;

	player->PLAYER_SPEED = m_filereader->paramFloat.find("PLAYER_SPEED")->second;
	player->PLAYER_DASH_SPEED = m_filereader->paramFloat.find("PLAYER_DASH_SPEED")->second;
	player->PLAYER_DASH_FRAME = m_filereader->paramInt.find("PLAYER_DASH_FRAME")->second;
	player->PLAYER_DASH_PAUSE_FRAME = m_filereader->paramInt.find("PLAYER_DASH_PAUSE_FRAME")->second;
	
	player->PLAYER_BULLET_DAMAGE = m_filereader->paramInt.find("PLAYER_BULLET_DAMAGE")->second;
	player->PLAYER_BULLET_SPEED = m_filereader->paramFloat.find("PLAYER_BULLET_SPEED")->second;
	player->PLAYER_BULLET_DISTANCE = m_filereader->paramInt.find("PLAYER_BULLET_DISTANCE")->second;
	player->PLAYER_BULLET_DELAY = m_filereader->paramInt.find("PLAYER_BULLET_DELAY")->second;
	
}

void GraphicsClass::InitializeMap() 
{
	floor = new staticobjclass("floor",m_D3D, gameObject::COLLIDER_BOX, gameObject::NO_COLLISION);
	floor->SetScale(D3DXVECTOR3(20, 0.1, 20));
	floor->SetPosition(D3DXVECTOR3(0, -5, 0));
	floor->SetRotation(D3DXVECTOR3(0, 45, 0));
	//m_GM->RegisterObjectToRender(temp);
	
	
	player = new playerclass(100, m_D3D);
	player->SetGameManager(m_GM);
	m_GM->RegisterObjectToRender(player);

	boss = new bossclass(30, 1, m_D3D, player);
	boss->SetPosition(D3DXVECTOR3(0, 0, 20));
	boss->SetGameManager(m_GM);
	m_GM->RegisterObjectToRender(boss);

	midPoint = (player->GetPosition() + boss->GetPosition()) / 2;
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

	//-------------------
	//   Frame Action
	//-------------------
	frame++;
	if (frame > 10000)
		frame = lastLeftClick = 0;

	
	//ADD BULLETS
	vector<projectileclass*> bossBullet = boss->Frame(frame);
	for (unsigned int i = 0; i < bossBullet.size(); i++)
		m_GM->RegisterObjectToRender(bossBullet[i]);

	m_GM->AlphaSort(m_Camera->GetPosition());
	if (frame % COLL_CHECK_RATE)
		m_GM->CheckCollision();

	//CAN CLICK AGAIN!
	if (frame - lastLeftClick > MOUSE_FRAME_RATE)
		lastLeftClick = 0;
	
	//UI
	if (!SetUI(mouseX, mouseY, fps, cpu))
		return false;

	//-------------------
	//  Input Handler
	//-------------------
	if (stdafx::RightMouseClicked(mousePress))
	{


	}

	if (stdafx::LeftMouseClicked(mousePress))
	{
		if (!lastLeftClick)
		{
			m_GM->RegisterObjectToRender(player->Fire( GetDirectionMouse(_mouseX,_mouseY) ));
			lastLeftClick = frame;
		}

	}

	//-------------
	//  player
	//-------------
	AutoMove();
	player->Move(key, frame);

	midPoint = (player->GetPosition() + boss->GetPosition()) / 2;
	float distance = stdafx::GetDistance(player->GetPosition(), boss->GetPosition());

	m_Camera->Move(midPoint, distance);
	

	//-------------
	//  object
	//-------------

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
	D3DXMATRIX temp, MatrixToFaceCamera;
	bool result;

	//-----------------------

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(player->GetPosition());
	m_Camera->GetBillBoardMatrix(MatrixToFaceCamera, midPoint);
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	//-----------------
	// Transformation
	//-----------------
	m_D3D->TurnOnAlphaBlending();

	//render floor first
	floor->GetModel()->Render(m_D3D->GetDeviceContext());
	floor->GetWorldMatrix(worldMatrix);
	m_LightShader->Render(m_D3D->GetDeviceContext(), floor->GetModel()->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix, floor->GetModel()->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());

	int size = m_GM->GetRenderObjectCount();
	for (int i = 0; i < size; i++)
	{
		gameObject* temp = m_GM->GetGameObject(i);
		temp->GetModel()->Render(m_D3D->GetDeviceContext());

		temp->GetWorldMatrix(worldMatrix);
		if (temp->GetName() == "player" || temp->GetName() == "boss")
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

	// Turn on the alpha blending before rendering the text.
	//m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	D3DXMatrixIdentity(&worldMatrix);
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}




float GraphicsClass::clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

void GraphicsClass::AutoMove()
{
	//Move AUTOMOVE objects
	int size = m_GM->GetRenderObjectCount();
	for (int i = size - 1; i >= 0; i--)
	{
		gameObject* temp = m_GM->GetGameObject(i);
		if (temp->objType == gameObject::AUTOMOVE)
		{
			projectileclass* bullet = (projectileclass*)temp;

			if (bullet->delay > 0)
			{
				bullet->delay--;
				bullet->Move(0.1);

			}
			else
			{
				bullet->Move(1);
				if (temp->CheckDestroy())
				{
					if (temp->GetName() == "bossbullet")
					{
						m_GM->UnregisterObjectToRender(temp);
						m_GM->RegisterToBossPool((projectileclass*)temp);
					}
					else if (temp->GetName() == "playerbullet")
					{
						m_GM->UnregisterObjectToRender(temp);
						m_GM->RegisterToPlayerPool((projectileclass*)temp);
					}
					else
						m_GM->RemoveObjectToRender(temp);
				}
			}
		}
	}
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
}