#pragma once
#ifndef _ITEMMANAGERCLASS_H_
#define _ITEMMANAGERCLASS_H_

#define ITEM_PHASE_COUNT 3

class playerclass;
class textfilereader;
class gameManager;
class uimanagerclass;

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
	void SetManagers(gameManager*, uimanagerclass*);

private:
	void SetItemPool();
	
	gameManager* GM;
	playerclass* player;
	textfilereader* itemparameters;
	uimanagerclass* UIM;

	vector<vector<Item>> itemPool;
};

#endif