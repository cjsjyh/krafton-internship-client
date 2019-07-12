////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
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

	return true;
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

int GraphicsClass::GetDirectionKey(char* keys)
{
	// 0 1 2
	// 7   3
	// 6 5 4
	//0 = W A
	if (keys[0] && keys[1])
		return 0;
	//2 = W D
	else if (keys[0] && keys[3])
		return 2;
	//4 = S D
	else if (keys[2] && keys[3])
		return 4;
	//6 = S A
	else if (keys[2] && keys[1])
		return 6;
	//1 = W
	else if (keys[0])
		return 1;
	//3 = D
	else if (keys[3])
		return 3;
	//5 = S
	else if (keys[2])
		return 5;
	//7 = A
	else if (keys[1])
		return 7;
	else
		return -1;
}


D3DXVECTOR3 GraphicsClass::GetDirectionMouse()
{
	float square, gradient;
	float adjustedX = mouseX, adjustedY = mouseY;
	float amount = 100;
	
	gradient = abs((float)mouseY) / abs(mouseX);
	gradient = abs(gradient - 1);
	amount *= clamp(1-gradient, 0.7, 1);
	
	if (mouseY < 0)
		amount *= -1;

	adjustedY += amount;

	square = adjustedX * adjustedX + adjustedY * adjustedY;
	square = sqrt(square);
	return D3DXVECTOR3(adjustedX / square, 0, adjustedY / square);
}

bool GraphicsClass::Frame(int _mouseX, int _mouseY, bool* mousePress, char* key)
{
	vector<gameObject*> coll1, coll2;
	bool result;

	mouseX = _mouseX - screenW/2;
	mouseY = -(_mouseY - screenH/2);
	
	//-------------------
	//   Frame Action
	//-------------------
	frame++;
	if (frame % COLL_CHECK_RATE)
		m_GM->CollisionManager(coll1, coll2);
	if (frame - lastLeftClick > MOUSE_FRAME_RATE)
		lastLeftClick = 0;

	if (frame > 10000)
		frame = lastLeftClick = 0;
	
	//Move AUTOMOVE objects
	int size = m_GM->GetObjectCount();
	for (int i = size-1; i >= 0 ; i--)
	{
		gameObject* temp = m_GM->GetGameObject(i);
		if (temp->objType == gameObject::AUTOMOVE)
		{
			projectile* bullet = (projectile*)temp;
			bullet->Move();
			if (temp->CheckDestroy())
				m_GM->UnregisterObject(temp);
		}
	}

	//-------------------
	//  Input Handler
	//-------------------
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

	//rightclick
	if (RightMouseClicked(mousePress))
	{


	}

	if (LeftMouseClicked(mousePress))
	{
		if (!lastLeftClick)
		{
			m_GM->RegisterObject(player->Fire( GetDirectionMouse() ));
			m_GM->RegisterObject(boss->Fire());

			lastLeftClick = frame;
		}

	}

	//-------------
	//  player
	//-------------
		
	for (int i = 0; i < sizeof(key); i++)
	{
		switch (key[i]) {
		case 'A':
			player->AdjustPosition(D3DXVECTOR3(-PLAYER_SPEED, 0, 0));
			m_Camera->AdjustPosition(D3DXVECTOR3(-PLAYER_SPEED, 0, 0));
			break;
		case 'S':
			player->AdjustPosition(D3DXVECTOR3(0, 0, -PLAYER_SPEED));
			m_Camera->AdjustPosition(D3DXVECTOR3(0, 0, -PLAYER_SPEED));
			break;
		case 'D':
			player->AdjustPosition(D3DXVECTOR3(PLAYER_SPEED, 0, 0));
			m_Camera->AdjustPosition(D3DXVECTOR3(PLAYER_SPEED, 0, 0));
			break;
		case 'W':
			player->AdjustPosition(D3DXVECTOR3(0, 0, PLAYER_SPEED));
			m_Camera->AdjustPosition(D3DXVECTOR3(0, 0, PLAYER_SPEED));
			break;
		}
	}
	//image direction
	int dir = GetDirectionKey(key);
	if(dir != -1)
		player->SetDirection(dir);
	switch (dir)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	default:
		break;
	}
	

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
	float rotx, roty, rotz;
	float x,y,z;


	D3DXMatrixRotationX(&MatrixToFaceCamera,45);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	D3DXVECTOR3 player_pos = player->GetPosition();
	m_Camera->Render(player_pos);
	

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	
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
			worldMatrix = MatrixToFaceCamera* worldMatrix;
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
	square = vec.x * vec.x + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, 0, vec.z / square);
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