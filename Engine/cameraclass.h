////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void Move(char*);
	void SetSpeed(float);

	void SetPosition(D3DXVECTOR3);
	void AdjustPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);
	void AdjustRotation(D3DXVECTOR3);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	void GetBillBoardMatrix(D3DXMATRIX&, D3DXVECTOR3);

private:
	D3DXVECTOR3 normalizeVec3(D3DXVECTOR3);
	void PrintVector3(D3DXVECTOR3);

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;

	float PLAYER_SPEED;
};

#endif