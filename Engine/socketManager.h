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
#define MAX_PLAYER_COUNT 2


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

	int bossHitCount;
	int playerHitCount;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;

		ar& playerPos;

		ar& mouseX;
		ar& mouseY;
		ar& mouseInput;
		ar& keyInput;

		ar& bossHitCount;
		ar& playerHitCount;
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
		ITEM_INFO,
	};

	socketManager();
	~socketManager();

	int Initialize();
	bool Shutdown();
	
	bool Frame(bool,playerInput*);
	MsgBundle* GetNewMessage();

public:
	int playerId;
	playerInput pInfo;
	std::queue<MsgBundle*> serverReadBuffer;

private:
	void ListenToServer();
	MsgBundle* receiveMessage(SOCKET);
	int sendMessage(SOCKET, void*, DataType);
	void CopyPlayerInfo(playerInput*, playerInput*);

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