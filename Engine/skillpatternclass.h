#pragma once
#ifndef _SKILLPATTERNCLASS_H_
#define _SKILLPATTERNCLASS_H_

class skillpatternclass
{
public:
	skillpatternclass();
	~skillpatternclass();

	static D3DXVECTOR3 FireAt(D3DXVECTOR3, D3DXVECTOR3);
	static vector<D3DXVECTOR3> FireInCircle(int);
	static vector<D3DXVECTOR3> FireInFan(int dirCount, int angle, D3DXVECTOR3, D3DXVECTOR3);

private:
};

#endif
