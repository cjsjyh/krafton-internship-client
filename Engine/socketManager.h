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

	playerInput(int _playerId, int _mouseX, int _mouseY, bool* _mouseInput, int* _keyInput)
		:playerId(_playerId), mouseX(_mouseX), mouseY(_mouseY)
	{
		for (int i = 0; i < sizeof(_mouseInput); i++)
			mouseInput[i] = _mouseInput[i];
		for (int i = 0; i < sizeof(keyInput) / sizeof(int); i++)
			keyInput[i] = _keyInput[i];
	}

	playerInput() {
	}

	~playerInput() {

	}
	int playerId;
	int playerPos[3];
	
	int mouseX, mouseY;
	bool mouseInput[3];
	int keyInput[10];

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& playerPos;

		ar& mouseX;
		ar& mouseY;
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
			playerHp[i] = -1;
		bossHp = -1;
		playerMaxHp = -1;
		bossMaxHp = -1;
	}

	hpInfo(int _playerId, int _bossHit, int _playerHit)
	{
		for (int i = 0; i < sizeof(playerHp) / sizeof(int); i++)
			playerHp[i] = -1;
		bossHp = -1;
		bossHitCount = -1;
		playerMaxHp = -1;
		bossMaxHp = -1;

		playerId = _playerId;
		bossHitCount = _bossHit;
		playerHitCount = _playerHit;
	}

	int playerId;
	int playerHp[2];
	int bossHp;
	int bossHitCount;
	int playerHitCount;

	int playerMaxHp;
	int bossMaxHp;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& playerHp;
		ar& bossHp;
		ar& bossHitCount;
		ar& playerHitCount;
		ar& playerMaxHp;
		ar& bossMaxHp;
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