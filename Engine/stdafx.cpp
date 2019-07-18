#include "stdafx.h"


float stdafx::GetDistance(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	float length = pow(vec1.x - vec2.x, 2) + pow(vec1.z - vec2.z, 2);
	float height = pow(vec1.y - vec2.y, 2);

	return sqrt(length + height);
}

D3DXVECTOR3 stdafx::normalizeVec3(D3DXVECTOR3 vec)
{
	float square;
	square = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	square = sqrt(square);
	return D3DXVECTOR3(vec.x / square, vec.y / square, vec.z / square);
}

void stdafx::PrintVector3(D3DXVECTOR3 vec)
{
	cout << "x: " + to_string(vec.x) << "y: " + to_string(vec.y) << "z: " + to_string(vec.z) << endl;
}


