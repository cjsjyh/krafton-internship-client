#include "stdafx.h"

#include <cstdlib>
#include <ctime>

#include "itemmanagerclass.h"

itemmanagerclass::itemmanagerclass()
{
	srand(unsigned int(time(NULL)));

	for (int i = 0; i < ITEM_PHASE_COUNT; i++)
	{
		vector<Item> temp2;
		itemPool.push_back(temp2);
	}
	SetItemPool();
}

void itemmanagerclass::SetItemPool()
{
	for (int i = 1; i < 6; i++)
	{
		Item temp;
		temp.name = to_string(i);
		temp.chosen = false;
		itemPool[0].push_back(temp);

		Item temp2;
		temp.name = to_string(i);
		temp.chosen = false;
		itemPool[1].push_back(temp2);
	}
	return;
}

string itemmanagerclass::ChooseItemFromPool(int phase)
{
	int index = rand() % itemPool[phase].size();
	return itemPool[phase][index].name;
}

void itemmanagerclass::SetItemUsed(int phase, string name)
{
	for (auto iter = itemPool[phase].begin(); iter != itemPool[phase].end(); iter++)
	{
		if ((*iter).name == name)
			(*iter).chosen = true;
	}
}
