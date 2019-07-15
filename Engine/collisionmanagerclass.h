#pragma once
#ifndef _COLLISIONMANAGER_CLASS_H_
#define _COLLISIONMANAGER_CLASS_H_

#include <vector>

class collisionmanagerclass
{
public:
	collisionmanagerclass();
	bool CollisionManager(gameManager*, vector<gameObject*>&, vector<gameObject*>&);

private:

	int CollisionHandler(gameManager*, gameObject*, gameObject*);
	bool CheckCollisionChannel(gameObject*, gameObject*);

	bool SimpleComplexCollision(gameObject*, gameObject*);
	bool SimpleBoxCollision(gameObject*, gameObject*);
	bool ComplexCollision(gameObject*, gameObject*);

	bool SimpleDetection(gameObject*, vector<D3DXVECTOR3>::iterator, float);
	vector <D3DXVECTOR3> ComplexCollisionInitialize(gameObject*);
};

#endif

