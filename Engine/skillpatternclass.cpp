#include "stdafx.h"

#include "skillpatternclass.h"

skillpatternclass::skillpatternclass()
{

}

skillpatternclass::~skillpatternclass()
{

}

D3DXVECTOR3 skillpatternclass::FireAt(D3DXVECTOR3 src, D3DXVECTOR3 dest)
{
	return stdafx::normalizeVec3(dest - src);
}

vector<D3DXVECTOR3> skillpatternclass::FireInCircle(int dir)
{
	D3DXMATRIX rotMatrix;
	D3DXVECTOR3 dirVec = D3DXVECTOR3(1, 0, 1);
	float angle;
	vector<D3DXVECTOR3> dirVectors;

	for (int i = 0; i < dir; i++)
	{
		dirVec = D3DXVECTOR3(1, 0, 1);
		angle = 360 / dir * i;
		angle *= 0.0174532925f;

		D3DXMatrixIdentity(&rotMatrix);
		D3DXMatrixRotationY(&rotMatrix, angle);
		D3DXVec3TransformCoord(&dirVec, &dirVec, &rotMatrix);

		dirVectors.push_back(stdafx::normalizeVec3(dirVec));
	}
	return dirVectors;
}

vector<D3DXVECTOR3> skillpatternclass::FireInFan(int dirCount, int angle, D3DXVECTOR3 src, D3DXVECTOR3 dest)
{
	vector<D3DXVECTOR3> dirVectors;
	D3DXVECTOR3 dirVec, middleVec;
	float angleleft, angleright;
	D3DXMATRIX rotleft, rotright;
	
	middleVec = stdafx::normalizeVec3(dest - src);
	
	//Even number of bullet
	if (dirCount % 2 == 0)
	{
		for (int i = 0; i < dirCount / 2; i++)
		{
			angleright = (angle / 2) + (angle * i);
			angleright *= 0.0174532925f;
			angleleft = (-angle / 2) + (-angle* i);
			angleleft *= 0.0174532925f;

			D3DXMatrixRotationY(&rotleft, angleleft);
			D3DXMatrixRotationY(&rotright, angleright);

			D3DXVec3TransformCoord(&dirVec, &middleVec, &rotleft);
			dirVectors.push_back(dirVec);
			D3DXVec3TransformCoord(&dirVec, &middleVec, &rotright);
			dirVectors.push_back(dirVec);
		}
	}
	//Odd number of bullet
	else
	{
		dirVectors.push_back(middleVec);
		for (int i = 1; i <= (dirCount - 1) / 2; i++)
		{
			angleright = angle * i;
			angleright *= 0.0174532925f;
			angleleft = -angle * i;
			angleleft *= 0.0174532925f;

			D3DXMatrixRotationY(&rotleft, angleleft);
			D3DXMatrixRotationY(&rotright, angleright);

			D3DXVec3TransformCoord(&dirVec, &middleVec, &rotleft);
			dirVectors.push_back(dirVec);
			D3DXVec3TransformCoord(&dirVec, &middleVec, &rotright);
			dirVectors.push_back(dirVec);
		}

	}

	return dirVectors;
}
