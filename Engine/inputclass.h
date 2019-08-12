#pragma once
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


#define DIRECTINPUT_VERSION 0x0800

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame(bool&);

	bool IsLMouseDown(); // 왼쪽 클릭
	bool IsRMouseDown(); // 오른쪽 클릭
	bool IsMMouseDown(); // 휠 클릭

	bool IsEscapePressed();

	bool IsKeyPressed(int);
	static bool IsKeyPressed(int* arr, int key);

	void GetMouseLocation(int&, int&);
	void GetMouseOffset(int&, int&);
	void GetScreenSize(int&, int&);

	static bool IsWASDKeyPressed(int* arr);
	static bool MouseNotClicked(bool* mouseInput);
	static bool RightMouseClicked(bool* mouseInput);
	static bool LeftMouseClicked(bool* mouseInput);

	int prevMouseX;
	int prevMouseY;
	int keyInput[10];
	int prevkeyInput[10];
	bool mouseInput[3];
	bool prevmouseInput[3];

private:
	bool ReadKeyboard();
	bool ReadMouse();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	POINT cursorPos;
	HWND hwnd;

	const int keyCode[KEY_NUM] = { DIK_W, DIK_A,DIK_S,DIK_D,DIK_SPACE, DIK_LSHIFT, DIK_RETURN, DIK_F };

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif