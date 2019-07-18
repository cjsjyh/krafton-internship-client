////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#define SCENE_COUNT 2

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
class D3DClass;
class inputclass;
class ModelClass;

class CameraClass;
class TextureShaderClass;
class TextClass;
class LightShaderClass;
class LightClass;

class gameObject;
class staticobjclass;
class playerclass;
class projectileclass;
class bossclass;

class gameManager;
class textfilereader;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float CAM_SENSITIVITY = 0.01f;

const int PLAYER_MODEL_COUNT = 8;

const int MOUSE_FRAME_RATE = 10;
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
	void InitializeParameters();
	void Shutdown();
	bool Frame(int, int, bool*, int*, int, int);
	bool Render();

	float clamp(float, float, float);

private:
	bool MouseNotClicked(bool*);
	bool RightMouseClicked(bool*);
	bool LeftMouseClicked(bool*);
	bool IsKeyPressed(char*);

	D3DXVECTOR3 GetDirectionMouse(int, int);
	void InitializeMap();
	void AutoMove();
	bool SetUI(int, int, int, int);

	textfilereader* m_filereader;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	
	playerclass* player;
	bossclass* boss;
	gameManager* m_GM;
	gameObject* floor;

	vector<ModelClass*> m_Model;
	

	D3DXVECTOR3 midPoint;
	int mouseX, mouseY;
	int frame;
	int screenW, screenH;
	int lastLeftClick;
};

#endif