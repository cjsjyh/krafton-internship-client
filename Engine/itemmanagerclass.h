#pragma once
#ifndef _ITEMMANAGERCLASS_H_
#define _ITEMMANAGERCLASS_H_

#define ITEM_PHASE_COUNT 3

class itemmanagerclass
{
public:
	typedef struct Item {
		string name;
		bool chosen;
	}Item;

	itemmanagerclass();

	string ChooseItemFromPool(int);
	void SetItemUsed(int, string);

private:
	void SetItemPool();
	

	vector<vector<Item>> itemPool;

};

#endif