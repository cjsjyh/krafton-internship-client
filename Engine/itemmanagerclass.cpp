#include "stdafx.h"

#include <cstdlib>
#include <ctime>

#include "gameManager.h"
#include "textfilereader.h"
#include "uimanagerclass.h"
#include "d3dclass.h"

#include "playerclass.h"
#include "staticobjclass.h"

#include "itemmanagerclass.h"

itemmanagerclass::itemmanagerclass()
{
	srand(unsigned int(time(NULL)));

	for (int i = 0; i < ITEM_PHASE_COUNT; i++)
	{
		vector<Item> temp2;
		itemPool.push_back(temp2);
	}
	player = 0;
	GM = 0;
	UIM = 0;
	tooltip = 0;
}

itemmanagerclass::~itemmanagerclass()
{

}

void itemmanagerclass::SetParameter(textfilereader* _itemparameters)
{
	itemparameters = _itemparameters;
	SetItemPool();
}

void itemmanagerclass::SetManagers(gameManager* _GM, uimanagerclass* _UIM, D3DClass* _device)
{
	device = _device;
	GM = _GM;
	UIM = _UIM;
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

void itemmanagerclass::DisplayTooltip(gameObject* item)
{
	if (tooltip)
	{
		GM->RemoveObjectToRender("tooltip",1);
		tooltip = 0;
	}
	tooltip = new staticobjclass("tooltip", device, gameObject::INTERACTION, gameObject::COLLIDER_BOX);
	((staticobjclass*)tooltip)->InitializeStatic2DItem("tip_",item->GetName());
	tooltip->SetPosition(item->GetPosition() + D3DXVECTOR3(0,5,0));
	tooltip->SetScale(D3DXVECTOR3(6, 2.5, 1));
	GM->RegisterObjectToRender(tooltip, 1);
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
			if ((*iter).chosen == false)
			{
				(*iter).chosen = true;
				SetItemEffect((*iter).name);
			}
		}
	}
}

void itemmanagerclass::SetItemEffect(string name)
{
	if (player == 0)
		player = (playerclass*)GM->GetGameObject("player");

	if (name == "shotgun")
	{
		player->SetPlayerAttackType(name);
		player->PLAYER_BULLET_COUNT = 3;
		player->PLAYER_BULLET_ANGLE = 10;
		player->PLAYER_BULLET_DISTANCE = 5;
		player->PLAYER_BULLET_DAMAGE = 5;
		player->PLAYER_BULLET_SPEED = 1;
		UIM->ReplaceUI("PLAYER_ATTACK_TYPE", "icon_shotgun.png");
	}
	else if (name == "sniper")
	{
		player->SetPlayerAttackType(name);
		player->PLAYER_BULLET_COUNT = 1;
		player->PLAYER_BULLET_ANGLE = 0;
		player->PLAYER_BULLET_DISTANCE = 100;
		player->PLAYER_BULLET_DAMAGE = 5;
		player->PLAYER_BULLET_SPEED = 3;
		UIM->ReplaceUI("PLAYER_ATTACK_TYPE", "icon_sniper.png");
	}
	else if (name == "berserk")
	{
		player->AddPlayerItem(name);

	}
	else if (name == "siege")
	{
		player->AddPlayerItem(name);
	}
	else if (name == "speedup")
	{
		player->AddPlayerItem(name);
	}

	return;
}
