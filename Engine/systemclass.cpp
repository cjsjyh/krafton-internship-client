////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "fpsclass.h"
#include "cpuclass.h"
#include "inputclass.h"
#include "ApplicationClass.h"
#include "socketManager.h"
#include <windows.h>

#include "playerclass.h"
#include "systemclass.h"

#include "socketInterface.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	
	m_Fps = 0;
	m_Cpu = 0;

	mouseX = mouseY = 0;
	//TEMP//
	m_Socket = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight, posX, posY;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight, posX, posY);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	m_Socket = new socketManager();
	if (!m_Socket)
	{
		std::cout << "Socket not initialized properly" << std::endl;
		return false;
	}
	m_Socket->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new ApplicationClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}
	
	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Socket)
	{
		delete m_Socket;
		m_Socket = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
	return;
}


bool SystemClass::Frame()
{
	bool result;
	bool IsKeyChanged;
	m_Fps->Frame();
	m_Cpu->Frame();
	//cout << "CPU: " + to_string(m_Cpu->GetCpuPercentage()) << " FPS: " + to_string(m_Fps->GetFps()) << endl;

	// Do the input frame processing.
	result = m_Input->Frame(IsKeyChanged);
	if (!result)
	{
		return false;
	}

	m_Input->GetMouseLocation(mouseX, mouseY);

	m_Socket->Frame(IsKeyChanged, WrapInput());
	
	//TEMP//
	int handleCount = 0;
	MsgBundle* newMsg = m_Socket->GetNewMessage();
	while (newMsg != NULL)
	{
		handleCount++;
		if (newMsg != NULL)
		{
			switch (newMsg->type)
			{
			case socketManager::PLAYER_INFO:
				playerInput* pInfo;
				pInfo = (playerInput*)(newMsg->ptr);

				if (pInfo->playerId >= 0 && pInfo->playerId < 2)
				{
					for (int i = 0; i < sizeof(pInfo->keyInput) / sizeof(int); i++)
						socketInterface::keyInput[pInfo->playerId][i] = pInfo->keyInput[i];
					for (int i = 0; i < sizeof(pInfo->mouseInput); i++)
						socketInterface::mouseInput[pInfo->playerId][i] = pInfo->mouseInput[i];
					for (int i = 0; i < 3; i++)
					{
						socketInterface::playerPos[pInfo->playerId][i] = pInfo->playerPos[i];
						socketInterface::mouseDirVec[pInfo->playerId][i] = pInfo->mouseDirVec[i];
					}
				}
				delete pInfo;
				break;

			case socketManager::BOSS_INFO:
				BossInfo* bInfo;
				bInfo = (BossInfo*)(newMsg->ptr);
				socketInterface::bossPatternQueue.push(bInfo->patternId);
				delete bInfo;
				break;

			case socketManager::HP_INFO:
				hpInfo* hInfo;
				hInfo = (hpInfo*)(newMsg->ptr);

				for (int i = 0; i < 2; i++)
				{
					socketInterface::playerHp[i] = hInfo->playerHp[i];
				}
				socketInterface::bossHp = hInfo->bossHp;
				delete hInfo;
				break;

			case socketManager::ITEM_INFO:

				break;
			}
		}
		delete newMsg;
		newMsg = 0;
		newMsg = m_Socket->GetNewMessage();
	}
	std::cout << "Handled " + std::to_string(handleCount) << std::endl;
	

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(mouseX,mouseY);
	if (!result)
	{
		return false;
	}
	
	return true;
}

playerInput* SystemClass::WrapInput()
{
	playerInput* temp = new playerInput;
	temp->playerId = socketInterface::playerId;

	for (int i = 0; i < sizeof(temp->keyInput) / sizeof(int); i++)
	{
		/*if (m_Input->keyInput[i] == DIK_LSHIFT)
		{
			if (socketInterface::UltiUsed)
				socketInterface::UltiUsed = false;
			else
				continue;
		}*/
		temp->keyInput[i] = m_Input->keyInput[i];
	}
	for (int i = 0; i < sizeof(temp->mouseInput); i++)
		temp->mouseInput[i] = m_Input->mouseInput[i];
	
	for (int i = 0; i < 3; i++)
	{
		temp->playerPos[i] = socketInterface::curPlayerPos[i];
		temp->mouseDirVec[i] = socketInterface::curPlayerDirVec[i];
	}

	return temp;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight, int& _posX, int& _posY)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 1200x800 resolution.
		screenWidth  = 1920;
		screenHeight = 1080;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		_posX = posX;
		_posY = posY;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}