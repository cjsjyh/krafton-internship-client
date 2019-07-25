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
class uimanagerclass;
class BitmapClass;

class gameObject;
class staticobjclass;
class playerclass;
class projectileclass;
class bossclass;

class itemmanagerclass;
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
	void InitializeBasic();
	void UninitializeBasic();
	void InitializeMap();
	void UninitializeMap();
	void InitializePlayerParameters();
	void InitializeBossParameters();
	void InitializeRewardMap(vector<string>);
	void UninitializeRewardMap();
	void Shutdown();
	bool Frame(int, int, bool*, int*, int, int);
	bool Render();

private:
	D3DXVECTOR3 GetDirectionMouse(int, int);
	
	bool SetUI(int, int, int, int);

	textfilereader* m_filereader;
	D3DClass* m_D3D;
	TextClass* m_Text;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	uimanagerclass* m_UIManager;
	
	playerclass* player;
	bossclass* boss;
	gameManager* m_GM;
	itemmanagerclass* m_IM;
	CameraClass* m_Camera;
	gameObject *floor, *rewardfloor;

	vector<ModelClass*> m_Model;
	
	D3DXMATRIX baseViewMatrix;
	D3DXVECTOR3 midPoint;
	int mouseX, mouseY;
	int frame;
	int screenW, screenH;
	int lastLeftClick;
	int sceneChangeFrame;
};

#endif