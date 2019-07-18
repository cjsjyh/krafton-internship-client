////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_



///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////



//////////////
// INCLUDES //
//////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsLMouseDown(); // 왼쪽 클릭
	bool IsRMouseDown(); // 오른쪽 클릭
	bool IsMMouseDown(); // 휠 클릭

	bool IsEscapePressed();
	bool IsKeyPressed(int);
	void GetMouseLocation(int&, int&);
	void GetMouseOffset(int&, int&);

	static bool IsWASDKeyPressed(int* arr);
	static bool IsKeyPressed(int* arr, char key);
	static bool MouseNotClicked(bool* mousePress);
	static bool RightMouseClicked(bool* mousePress);
	static bool LeftMouseClicked(bool* mousePress);

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

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif