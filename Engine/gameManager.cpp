#include "gameManager.h"
gameManager::gameManager()
{
	return;
}

gameManager::~gameManager()
{

}

void gameManager::RegisterObject(gameObject *item)
{
	gameobjects.push_back(item);
	cout << "manager size: " << gameobjects.size() << endl;
	return;
}

void gameManager::UnregisterObject(gameObject *item)
{
	int index = FindObjectIndex(item);
	gameobjects.erase(gameobjects.begin() + index);
	cout << "manager size: " << gameobjects.size() << endl;
	return;
}

int gameManager::FindObjectIndex(gameObject *item)
{
	vector<gameObject*>::iterator itr = find(gameobjects.begin(), gameobjects.end(), item);
	return distance(gameobjects.begin(), itr);
}

int gameManager::GetObjectCount()
{
	return gameobjects.size();
}

gameObject* gameManager::GetGameObject(int index)
{
	return gameobjects[index];
}

bool gameManager::CollisionManager(vector<gameObject*> &item1, vector<gameObject*> &item2)
{
	//check all objects if they collide and return true or false.
	//if there are objects colliding, add them to the vector
	int size = GetObjectCount();
	for (int i = 0; i < size-1; i++) {
		vector<gameManager::coord> obj1CollPts = GetColliderCenter(gameobjects[i]);
		for (int j = i + 1; j < size; j++) {
			vector<gameManager::coord> obj2CollPts = GetColliderCenter(gameobjects[j]);
			if (DetectCollision(obj1CollPts, obj2CollPts))
			{
				item1.push_back(gameobjects[i]);
				item2.push_back(gameobjects[j]);
			}
		}
	}
	
	if (item1.size() == 0)
		return false;
	else
		return true;
}

bool gameManager::DetectCollision(vector<gameManager::coord> obj1, vector<gameManager::coord> obj2)
{
	vector<gameManager::coord>::iterator iter1, iter2;
	for (iter1 = obj1.begin(); iter1 < obj1.end(); iter1++)
	{
		for (iter2 = obj2.begin(); iter2 < obj2.end(); iter2++)
		{
			float result = pow(iter1->x - iter2->x,2) + pow(iter1->y - iter2->y,2) + pow(iter1->z - iter2->z,2);
			if (result <= pow(COLLIDER_SIZE * 2, 2)) {
				cout << "HIT!!" << endl;
				return true;
			}
		}
	}
	return false;
}

vector <gameManager::coord> gameManager::GetColliderCenter(gameObject* obj)
{
	vector <gameManager::coord> points;
	float x, y, z;
	float w, h, l;
	obj->GetPosition(x, y, z);
	obj->GetScale(w, h, l);
	
	for (int i = 1; i < (w/COLLIDER_SIZE)*2; i++) {
		for (int j = 1; j < (h/COLLIDER_SIZE)*2; j++) {
			for (int k = 1; k < (l/COLLIDER_SIZE)*2; k++) {
				coord temp, temp2;
				temp.x = x - w + i * COLLIDER_SIZE;
				temp.y = y - h + j * COLLIDER_SIZE;
				temp.z = z - l + k * COLLIDER_SIZE;
				points.push_back(temp);
			}
		}
	}

	return points;
}