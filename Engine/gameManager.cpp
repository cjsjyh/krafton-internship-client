#include "gameManager.h"
gameManager::gameManager()
{
	collider_size = 0.5f;
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
			float min_distance = gameobjects[i]->collider_size + gameobjects[j]->collider_size;
			if (DetectCollision(obj1CollPts, obj2CollPts, min_distance))
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

bool gameManager::DetectCollision(vector<gameManager::coord> obj1, vector<gameManager::coord> obj2, float distance)
{
	vector<gameManager::coord>::iterator iter1, iter2;
	for (iter1 = obj1.begin(); iter1 < obj1.end(); iter1++)
	{
		for (iter2 = obj2.begin(); iter2 < obj2.end(); iter2++)
		{
			float result = pow(iter1->x - iter2->x,2) + pow(iter1->y - iter2->y,2) + pow(iter1->z - iter2->z,2);
			if (result <= pow(distance, 2)) {
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
	//from object
	float minVertex;
	obj->GetPosition(x, y, z);
	obj->GetScale(w, h, l);
	minVertex = min(min(w, h), l);
	minVertex -= collider_size;

	for (int i = 1; i < (w/collider_size)*2; i+=2) {
		for (int j = 1; j < (h/ collider_size)*2; j+=2) {
			for (int k = 1; k < (l/ collider_size)*2; k+=2) {
				coord temp;
				float maxVertex;
				temp.x = x - w + i * collider_size;
				temp.y = y - h + j * collider_size;
				temp.z = z - l + k * collider_size;
				maxVertex = max(max(abs(temp.x), abs(temp.y)), abs(temp.z));
				if (maxVertex < minVertex) {
					cout << "skip" << endl;
					continue;
				}
				points.push_back(temp);
			}
		}
	}

	return points;
}