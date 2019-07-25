#include "stdafx.h"

#include <cstdlib>
#include <ctime>

#include "playerclass.h"
#include "textfilereader.h"

#include "itemmanagerclass.h"

itemmanagerclass::itemmanagerclass()
{
	srand(unsigned int(time(NULL)));

	for (int i = 0; i < ITEM_PHASE_COUNT; i++)
	{
		vector<Item> temp2;
		itemPool.push_back(temp2);
	}
}

itemmanagerclass::~itemmanagerclass()
{

}

void itemmanagerclass::SetParameter(textfilereader* _itemparameters)
{
	itemparameters = _itemparameters;
	SetItemPool();
}

void itemmanagerclass::SetPlayer(playerclass* _player)
{
	player = _player;
}

void itemmanagerclass::SetItemPool()
{
	for (int i = 0; i < itemparameters->ItemNames.size(); i++)
	{
		Item temp;
		temp.name = itemparameters->ItemNames[i];
		temp.chosen = false;
		itemPool[0].push_back(temp);
	}
	return;
}

vector<string> itemmanagerclass::ChooseItemFromPool(int count, int phase)
{
	vector<string> temp;
	int index;
	bool flag;

	if (count > itemPool[phase].size())
		count = itemPool[phase].size();

	for (int i = 0; i < count; i++)
	{
		flag = false;
		while (!flag)
		{
			flag = true;
			index = rand() % itemPool[phase].size();
			for (int i = 0; i < temp.size(); i++)
			{
				if (temp[i] == itemPool[phase][index].name && !itemPool[phase][index].chosen)
					flag = false;
			}
		}
		temp.push_back(itemPool[phase][index].name);
	}
	return  temp;
}

void itemmanagerclass::SetItemUsed(string name, int phase)
{
	for (auto iter = itemPool[phase].begin(); iter != itemPool[phase].end(); iter++)
	{
		if ((*iter).name == name)
		{
			(*iter).chosen = true;
			SetItemEffect((*iter).name);
		}
	}
}

void itemmanagerclass::SetItemEffect(string name)
{
	if (name == "shotgun")
	{

	}
	else if (name == "sniper")
	{

	}
	else if (name == "berserk")
	{

	}

	return;
}
