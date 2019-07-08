////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	
	m_Text = 0;
	
	m_Model = 0;
	m_Model2 = 0;
	m_TextureShader = 0;

	m_GM = 0;

	D3DXMatrixIdentity(&cam_rotX);
	D3DXMatrixIdentity(&cam_rotY);
	frame = 0;


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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

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

	//-------------
	//   object
	//-------------

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/kraftonjpg.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
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

	player = new gameObject("player",m_Model);
	player->SetScale(2, 2, 2);
	m_GM->RegisterObject(player);

	temp = new gameObject("floor",m_Model2);
	temp->SetScale(10, 0.5, 10);
	temp->SetPosition(0, -5, 0);
	m_GM->RegisterObject(temp);
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

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
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


bool GraphicsClass::Frame(int mouseX, int mouseY, int offsetX, int offsetY, bool* mousePress, char* key)
{
	D3DXMATRIX temp_rotY, temp_rotX;
	bool result;
	//-------------
	//   UI
	//-------------

	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the Keyboard Input as UI.
	if (key[0])
		result = m_Text->SetKeyInput(key, m_D3D->GetDeviceContext());
	else
		result = m_Text->SetKeyInput("", m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//rightclick
	if (RightMouseClicked(mousePress))
	{
		//-------------
		//  camera
		//-------------
		switch (key[0]) {
		case 'A':
			m_Camera->AdjustPosition(-CAM_SPEED, 0, 0);
			break;
		case 'S':
			m_Camera->AdjustPosition(0, 0, -CAM_SPEED);
			break;
		case 'D':
			m_Camera->AdjustPosition(CAM_SPEED, 0, 0);
			break;
		case 'W':
			m_Camera->AdjustPosition(0, 0, CAM_SPEED);
			break;
		}
	}
	else if(MouseNotClicked(mousePress))
	{
		//-------------
		//  player
		//-------------
		
		switch (key[0]) {
		case 'A':
			player->AdjustPosition(-PLAYER_SPEED, 0, 0);
			break;
		case 'S':
			player->AdjustPosition(0, -PLAYER_SPEED,0);
			break;
		case 'D':
			player->AdjustPosition(PLAYER_SPEED, 0, 0);
			break;
		case 'W':
			player->AdjustPosition(0, PLAYER_SPEED, 0);
			break;
		}
		
	}

	if (RightMouseClicked(mousePress) && offsetX)
	{
		float temp = -((float)offsetX/screenW) * 360 * D3DX_PI *CAM_SENSITIVITY;
		D3DXMatrixRotationY(&temp_rotY, temp);
		cam_rotY = cam_rotY * temp_rotY;
	}
	if (RightMouseClicked(mousePress) && offsetY)
	{
		float temp = -((float)offsetY/screenH) * 360 * D3DX_PI *CAM_SENSITIVITY;
		D3DXMatrixRotationX(&temp_rotX, temp);
		cam_rotX = cam_rotX * temp_rotX;
	}

	//-------------
	//  object
	//-------------
	
	// Render the graphics scene.
	result = Render(cam_rotX, cam_rotY);
	if(!result)
	{
		return false;
	}
	
	return true;
}



bool GraphicsClass::Render(D3DXMATRIX cam_rotX, D3DXMATRIX cam_rotY)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX temp;

	vector<gameObject*> coll1, coll2;
	bool result;
	float rotx, roty, rotz;
	float x,y,z;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	viewMatrix = cam_rotX * cam_rotY * viewMatrix;
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());
	m_Model2->Render(m_D3D->GetDeviceContext());

	//-----------------
	// Transformation
	//-----------------

	if (frame++ > 3)
	{
		frame = 0;
		m_GM->CollisionManager(coll1, coll2);
	}

	int size = m_GM->GetObjectCount();
	for (int i = 0; i < size; i++)
	{	
		gameObject* temp = m_GM->GetGameObject(i);
		temp->GetWorldMatrix(worldMatrix);
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), temp->GetModel()->GetIndexCount(), worldMatrix, 
									viewMatrix, projectionMatrix, temp->GetModel()->GetTexture(), m_Light->GetDirection(),
									m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	
		if (!result)
		{
			return false;
		}
	}

	//
	


	//---------------------
	//   TEXT
	//---------------------

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
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