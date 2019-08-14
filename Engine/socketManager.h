#pragma once
#ifndef _SOCKETMANAGER_H_
#define _SOCKETMANAGER_H_

#include <mutex>
#include <thread>
#include <queue>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
using namespace boost::iostreams;


#define BUFFER_SIZE 512


class playerInput
{

public:
	friend class boost::serialization::access;

	playerInput(int _playerId, int* _mouseDirVec, bool* _mouseInput, int* _keyInput)
		:playerId(_playerId)
	{
		for (int i = 0; i < sizeof(_mouseInput); i++)
			mouseInput[i] = _mouseInput[i];
		for (int i = 0; i < sizeof(keyInput) / sizeof(int); i++)
			keyInput[i] = _keyInput[i];
		for (int i = 0; i < 3; i++)
			mouseDirVec[i] = _mouseDirVec[i];
	}

	playerInput() {
	}

	~playerInput() {

	}
	int playerId;
	float playerPos[3];
	
	float mouseDirVec[3];
	bool mouseInput[3];
	int keyInput[10];

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& playerPos;

		ar& mouseDirVec;
		ar& mouseInput;
		ar& keyInput;
	}
};

class hpInfo {
public:
	friend class boost::serialization::access;

	hpInfo() {
		playerId = -1;
		bossHitCount = -1;
		playerHitCount = -1;
		for (int i = 0; i < sizeof(playerHp) / sizeof(int); i++)
		{
			playerHp[i] = -1;
			playerHeal[i] = 0;
		}
		bossHp = -1;
		bossHeal = 0;
	}

	hpInfo(int _playerId, int _bossHit, int _playerHit)
	{
		for (int i = 0; i < sizeof(playerHp) / sizeof(int); i++)
		{
			playerHp[i] = -1;
			playerHeal[i] = 0;
		}
		bossHp = -1;
		playerId = _playerId;
		bossHitCount = _bossHit;
		playerHitCount = _playerHit;
		bossHeal = 0;
	}

	int playerId;
	int playerHp[2];
	int playerHitCount;
	int playerHeal[2];

	int bossHp;
	int bossHitCount;
	int bossHeal;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& playerHp;
		ar& bossHp;
		ar& bossHitCount;
		ar& playerHitCount;
		ar& playerHeal;
		ar& bossHeal;
	}
};

class ItemInfo {
public:
	friend class boost::serialization::access;

	ItemInfo() {
		playerId = -1;
		itemId = -1;
	}

	~ItemInfo() {

	}

	int playerId;
	int itemId;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& itemId;
	}
};

class BossInfo {
public:
	friend class boost::serialization::access;

	BossInfo() {
		patternId = 0;
	}

	BossInfo(int _patternId) {
		patternId = _patternId;
	}

	

	~BossInfo() {

	}

	int patternId;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& patternId;
	}
};

class InitialParamBundle {
public:
	friend class boost::serialization::access;

	InitialParamBundle()
	{
		playerMaxHp = -1;
		bossMaxHp = -1;
		bossPhase2Hp = -1;
		bossPhase3Hp = -1;
	}

	InitialParamBundle(int _pMaxHp, int _bMaxHp, int _b2Hp, int _b3Hp) {
		playerMaxHp = _pMaxHp;
		bossMaxHp = _bMaxHp;
		bossPhase2Hp = _b2Hp;
		bossPhase3Hp = _b3Hp;
	}
	~InitialParamBundle() {

	}

	int playerMaxHp;
	int bossMaxHp;
	int bossPhase2Hp;
	int bossPhase3Hp;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerMaxHp;
		ar& bossMaxHp;
		ar& bossPhase2Hp;
		ar& bossPhase3Hp;
	}
};

class MsgBundle
{
public:
	int type;
	void* ptr;
};

class socketManager
{
public:
	enum DataType
	{
		PLAYER_INFO,
		BOSS_INFO,
		HP_INFO,
		ITEM_INFO,
		PARAM_INFO,
	};

	enum ItemType
	{
		ACTIVE_SHOTGUN,
		ACTIVE_SNIPER,
		ACTIVE_BASIC,
		PASSIVE_RANGE,
		PASSIVE_TIME,
	};

	socketManager();
	~socketManager();

	int Initialize();
	bool Shutdown();
	
	bool Frame(bool,playerInput*);
	MsgBundle* GetNewMessage();

public:
	playerInput pInfo;
	std::queue<MsgBundle*> serverReadBuffer;

private:
	void ListenToServer();
	MsgBundle* receiveMessage(SOCKET);
	int sendMessage(SOCKET, void*, DataType);

	void CopyPlayerInfo(playerInput*, playerInput*);
	void CopyHpInfo(hpInfo*, hpInfo*);
	void CopyInitialParamBundle(InitialParamBundle*, InitialParamBundle*);
	void CopyItemInfo(ItemInfo*, ItemInfo*);
	void CopyBossInfo(BossInfo*, BossInfo*);

	void PrintPlayerInput(playerInput*);

private:
	char sendBuffer[BUFFER_SIZE];
	char recvBuffer[BUFFER_SIZE];
	std::vector<int> delimiterIndex;
	
	std::mutex* threadLock;

	SOCKET ConnectSocket;
	//InputClass* m_Input;
	int count=0;
};


#endif