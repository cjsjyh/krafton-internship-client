#pragma once
#ifndef _ITEMMANAGERCLASS_H_
#define _ITEMMANAGERCLASS_H_

#define ITEM_PHASE_COUNT 3

class playerclass;
class textfilereader;

class itemmanagerclass
{
public:
	typedef struct Item {
		string name;
		bool chosen;
	}Item;

	itemmanagerclass();
	~itemmanagerclass();

	vector<string> ChooseItemFromPool(int, int);
	void SetItemUsed(string, int);
	void SetItemEffect(string);

	void SetParameter(textfilereader*);
	void SetPlayer(playerclass* _player);

private:
	void SetItemPool();
	
	playerclass* player;
	textfilereader* itemparameters;
	vector<vector<Item>> itemPool;
};

#endif