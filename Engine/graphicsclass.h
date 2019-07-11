////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"

#include "cameraclass.h"
#include "textureshaderclass.h"
#include "textclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "staticobjclass.h"
#include "modelclass.h"
#include "gameObject.h"
#include "playerclass.h"
#include "projectile.h"
#include "inputclass.h"
#include "gameManager.h"

#include "iostream"
#include <string>
#include <vector>
#include <math.h>
using namespace std;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float CAM_SENSITIVITY = 0.01f;

const int PLAYER_MODEL_COUNT = 8;

const int MOUSE_FRAME_RATE = 100;
const int COLL_CHECK_RATE = 10;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, bool*, char*);
	bool Render();

private:
	bool MouseNotClicked(bool*);
	bool RightMouseClicked(bool*);
	bool LeftMouseClicked(bool*);
	bool IsKeyPressed(char*);

	int GetDirectionKey(char*);
	D3DXVECTOR3 GetDirectionMouse();

	//void ChangePlayerModel();

	void InitializeMap();

	void PrintVector3(D3DXVECTOR3 vec);

	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	playerclass* player;

	vector<ModelClass*> m_Model;
	gameManager* m_GM;

	int mouseX, mouseY;
	int frame;
	int screenW, screenH;
	int lastLeftClick;
};

#endif