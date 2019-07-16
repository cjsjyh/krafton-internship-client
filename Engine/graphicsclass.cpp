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
	m_Camera->Render();
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
	m_filereader->ReadFile("../Engine/data/parameter.csv");

	player->SetSpeed(m_filereader->params.find("player_speed")->second);
	m_Camera->SetSpeed(m_filereader->params.find("player_speed")->second);
}

void GraphicsClass::InitializeMap() 
{
	gameObject* temp;
	temp = new staticobjclass("floor",m_D3D, gameObject::COLLIDER_BOX, gameObject::NO_COLLISION);
	temp->SetScale(D3DXVECTOR3(20, 0.1, 20));
	temp->SetPosition(D3DXVECTOR3(0, -5, 0));
	temp->SetRotation(D3DXVECTOR3(0, 45, 0));
	m_GM->RegisterObject(temp);
	
	
	player = new playerclass(100, m_D3D);
	m_GM->RegisterObject(player);

	boss = new bossclass(30, 1, m_D3D, player);
	boss->SetPosition(D3DXVECTOR3(0, 0, 20));
	m_GM->RegisterObject(boss);
	

}

void GraphicsClass::PrintVector3(D3DXVECTOR3 vec)
{
	cout << "x: " + to_string(vec.x) << "y: " + to_string(vec.y) << "z: " + to_string(vec.z) << endl;
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

bool GraphicsClass::MouseNotClicked(bool* mousePress)
{
	if (!mousePress[0] && !mousePress[1] && !mousePress[2])
		return true;
	return false;
}

bool GraphicsClass::RightMouseClicked(bool* mousePress)
{
	if (mousePress[1])
		return true;
	return false;
}

bool GraphicsClass::LeftMouseClicked(bool* mousePress)
{
	if (mousePress[0])
		return true;
	return false;
}

bool GraphicsClass::IsKeyPressed(char* arr)
{
	for (int i = 0; i < sizeof(arr); i++)
		if (arr[i] != 0)
			return true;
	return false;
}


D3DXVECTOR3 GraphicsClass::GetDirectionMouse()
{
	float square, gradient;
	float adjustedX = mouseX, adjustedY = mouseY;
	float amount = 100;
	
	gradient = abs((float)mouseY) / abs(mouseX);
	gradient = abs(gradient - 1);

	if (abs(1 - gradient) > 0.2)
		amount *= clamp(abs(1 - gradient), 0.6, 1);
	else
		amount *= (1 - gradient);
	
	if (mouseY < 0)
		amount *= -1;
	adjustedY += amount;

	square = adjustedX * adjustedX + adjustedY * adjustedY;
	square = sqrt(square);
	return D3DXVECTOR3(adjustedX / square, 0, adjustedY / square);
}

bool GraphicsClass::Frame(int _mouseX, int _mouseY, bool* mousePress, char* key, int fps, int cpu)
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
		m_GM->RegisterObject(bossBullet[i]);

	if (frame % COLL_CHECK_RATE)
		m_GM->CheckCollision();

	//CAN CLICK AGAIN!
	if (frame - lastLeftClick > MOUSE_FRAME_RATE)
		lastLeftClick = 0;
	
	//UI
	if (!SetUI(mouseX, mouseY, fps, cpu, key))
		return false;

	//-------------------
	//  Input Handler
	//-------------------
	if (RightMouseClicked(mousePress))
	{


	}

	if (LeftMouseClicked(mousePress))
	{
		if (!lastLeftClick)
		{
			m_GM->RegisterObject(player->Fire( GetDirectionMouse() ));
			lastLeftClick = frame;
		}

	}

	//-------------
	//  player
	//-------------
	AutoMove();
	player->Move(key);
	m_Camera->Move(key);
	

	

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
	m_Camera->Render();
	m_Camera->GetBillBoardMatrix(MatrixToFaceCamera, player->GetPosition());

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	//-----------------
	// Transformation
	//-----------------
	m_D3D->TurnOnAlphaBlending();

	int size = m_GM->GetObjectCount();
	for (int i = 0; i < size; i++)
	{
		gameObject* temp = m_GM->GetGameObject(i);
		//¹Ù²Þ
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

D3DXVECTOR3 GraphicsClass::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, vec.y/square, vec.z / square);
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
	int size = m_GM->GetObjectCount();
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
					m_GM->UnregisterObject(temp);
			}
		}
	}
}

bool GraphicsClass::SetUI(int mouseX, int mouseY, int fps, int cpu, char* key)
{
	bool result;
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the Keyboard Input as UI.
	if (IsKeyPressed(key))
		result = m_Text->SetKeyInput("P", m_D3D->GetDeviceContext());
	else
		result = m_Text->SetKeyInput("", m_D3D->GetDeviceContext());
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