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
#include "modelclass.h"
#include "textureshaderclass.h"
#include "textclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "gameObject.h"

#include "iostream"
#include <string>
#include <vector>
using namespace std;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float CAM_SENSITIVITY = 0.01f;


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
	bool Frame(int, int, int, int, bool*, char*);
	bool Render(D3DXMATRIX, D3DXMATRIX);

private:
	bool MouseNotClicked(bool*);
	bool RightMouseClicked(bool*);
	bool LeftMouseClicked(bool*);

	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model, *m_Model2;
	TextClass* m_Text;
	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	vector <gameObject> gameObjects;
	gameObject* floor;

	D3DXMATRIX cam_rotY, cam_rotX;
	int frame;

	int screenW, screenH;
};

#endif