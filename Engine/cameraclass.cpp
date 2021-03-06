#include "stdafx.h"

#include <cstdlib>
#include <ctime>

#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	viewPoint.x = 0;
	viewPoint.y = 0;
	viewPoint.z = 0;

	shakeFrame = 0;
	shakePower = 1;
	
	srand((unsigned int)time(NULL));
}

CameraClass::~CameraClass()
{

}

void CameraClass::Move(float distance)
{
	if (distance < 30)
		SetPosition(D3DXVECTOR3(viewPoint.x, viewPoint.y + 30, viewPoint.z - 30));
	else
		SetPosition(D3DXVECTOR3(viewPoint.x, (viewPoint.y + 30) * (distance / 30), (viewPoint.z - 30) * (distance / 30)));
}

void CameraClass::SetPosition(D3DXVECTOR3 pos)
{
	m_positionX = pos.x;
	m_positionY = pos.y;
	m_positionZ = pos.z;
	return;
}

void CameraClass::AdjustPosition(D3DXVECTOR3 offset)
{
	m_positionX += offset.x;
	m_positionY += offset.y;
	m_positionZ += offset.z;
	return;
}


void CameraClass::SetRotation(D3DXVECTOR3 _rot)
{
	m_rotationX = _rot.x;
	m_rotationY = _rot.y;
	m_rotationZ = _rot.z;
	return;
}

void CameraClass::AdjustRotation(D3DXVECTOR3 _rotOffset)
{
	m_rotationX += _rotOffset.x;
	m_rotationY += _rotOffset.y;
	m_rotationZ += _rotOffset.z;
	return;
}

void CameraClass::SetViewPoint(D3DXVECTOR3 _viewpt)
{
	viewPoint.x = _viewpt.x;
	viewPoint.y = _viewpt.y;
	viewPoint.z = _viewpt.z;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

D3DXVECTOR3 CameraClass::GetViewPoint()
{
	return viewPoint;
}

void CameraClass::SetCameraShake(int _duration, float _strength)
{
	shakeFrame = _duration;
	shakePower = _strength;
}


void CameraClass::Render(D3DXVECTOR3 camViewPt)
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;
	float tempX = 0;
	float tempY = 0;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	if (shakeFrame-- > 0)
	{
		tempX = (float)(rand() % 200 - 100) / 100;
		if (tempX == 0)
			tempX = 1;
		tempX *= shakePower;
	}

	// Setup the position of the camera in the world.
	position.x = m_positionX + tempX;
	position.y = m_positionY + tempY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	//lookAt.x = camViewPt.x - m_positionX;
	//lookAt.y = camViewPt.y - m_positionY;
	//lookAt.z = camViewPt.z - m_positionZ;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::GetBillBoardMatrix(D3DXMATRIX& billBoardMatrix)
{
	float yaw, pitch, rad_yaw, rad_pitch;
	D3DXMATRIX rot_yaw, rot_pitch;
	D3DXVECTOR3 tempvec = normalizeVec3(viewPoint - GetPosition());

	yaw = atan2(tempvec.x, tempvec.z) * (180.0 / D3DX_PI);
	rad_yaw = yaw * 0.0174532925f;
	pitch = asin(-1 * tempvec.y) * (180.0 / D3DX_PI);
	rad_pitch = pitch * 0.0174532925f;

	D3DXMatrixIdentity(&rot_yaw);
	D3DXMatrixIdentity(&rot_pitch);
	D3DXMatrixRotationY(&rot_yaw, rad_yaw);
	D3DXMatrixRotationX(&rot_pitch, rad_pitch);

	billBoardMatrix = rot_pitch;
}

D3DXVECTOR3 CameraClass::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, vec.y / square, vec.z / square);
}