////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "socketInterface.h"

#include "inputclass.h"


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND h, int screenWidth, int screenHeight)
{
	HRESULT result;

	hwnd = h;
	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


bool InputClass::Frame(bool& IsKeyChanged)
{
	bool result, flag;
	GetCursorPos(&cursorPos);
	ScreenToClient(hwnd, &cursorPos);

	
	m_mouseX = cursorPos.x;
	m_mouseY = cursorPos.y;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }
	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	memset(keyInput, 0, sizeof(keyInput));
	for (int i = 0; i < KEY_NUM; i++) {
		if (IsKeyPressed(keyCode[i])) {
			keyInput[i] = keyCode[i];
		}
	}
	mouseInput[0] = IsLMouseDown();
	mouseInput[1] = IsRMouseDown();
	mouseInput[2] = IsMMouseDown();
	

	//COMPARE WITH PREVIOUS INPUT AND SET
	IsKeyChanged = false;
	for (int i = 0; i < KEY_NUM; i++) {
		if (keyInput[i] != prevkeyInput[i])
		{
			IsKeyChanged = true;
			prevkeyInput[i] = keyInput[i];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (mouseInput[i] != prevmouseInput[i])
		{
			IsKeyChanged = true;
			prevmouseInput[i] = mouseInput[i];
		}
	}
	//mouse가 눌려지고 있는 상태에서 마우스좌표만 변한경우
	flag = false;
	if (IsKeyChanged == false)
	{
		for (int i = 0; i < 3; i++)
			if (mouseInput[i] == true)
				flag = true;
		if (flag == true)
		{
			if (m_mouseX != prevMouseX || m_mouseY != prevMouseY)
				IsKeyChanged = true;
		}
	}
	prevMouseX = m_mouseX;
	prevMouseY = m_mouseY;

	return true;
}


bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)& m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)& m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::GetMouseOffset(int &x, int &y)
{
	x = m_mouseState.lX;
	y = m_mouseState.lY;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

void InputClass::GetScreenSize(int& screenWidth, int& screenHeight)
{
	screenWidth = m_screenWidth;
	screenHeight = m_screenHeight;
	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsKeyPressed(int key)
{
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsWASDKeyPressed(int* arr)
{
	for (int i = 0; i < 4; i++)
		if (arr[i] != 0)
			return true;
	return false;
}

bool InputClass::IsKeyPressed(int* arr, int key)
{
	for (int i = 0; i < KEY_NUM; i++)
		if (arr[i] == key)
			return true;
	return false;
}

bool InputClass::IsAnyKeyPressed(int* arr)
{
	for (int i = 0; i < KEY_NUM; i++)
		if (arr[i] != 0)
			return true;
	return false;
}

bool InputClass::IsLMouseDown()
{
	if (m_mouseState.rgbButtons[0] & 0x80)
		return true;
	return false;
}

bool InputClass::IsRMouseDown()
{
	if (m_mouseState.rgbButtons[1] & 0x80)
		return true;
	return false;
}

bool InputClass::IsMMouseDown()
{
	if (m_mouseState.rgbButtons[2] & 0x80)
		return true;
	return false;
}

bool InputClass::MouseNotClicked(bool* mouseInput)
{
	if (!mouseInput[0] && !mouseInput[1] && !mouseInput[2])
		return true;
	return false;
}

bool InputClass::RightMouseClicked(bool* mouseInput)
{
	if (mouseInput[1])
		return true;
	return false;
}

bool InputClass::LeftMouseClicked(bool* mouseInput)
{
	if (mouseInput[0])
		return true;
	return false;
}