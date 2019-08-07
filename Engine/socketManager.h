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

//class InputClass;

class playerInfo
{

public:
	friend class boost::serialization::access;

	playerInfo(int _playerId, int _mouseX, int _mouseY, bool* _mouseInput, int* _keyInput)
		:playerId(_playerId), mouseX(_mouseX), mouseY(_mouseY)
	{
		for (int i = 0; i < sizeof(_mouseInput); i++)
			mouseInput[i] = _mouseInput[i];
		for (int i = 0; i < sizeof(keyInput) / sizeof(int); i++)
			keyInput[i] = _keyInput[i];
	}

	playerInfo() {
	}

	~playerInfo() {

	}
	int playerId;
	int playerPos_x, playerPos_y, playerPos_z;
	
	int mouseX, mouseY;
	bool mouseInput[3];
	int keyInput[10];

	int bossHitCount;
	int playerHitCount;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;

		ar& playerPos_x;
		ar& playerPos_y;
		ar& playerPos_z;

		ar& mouseX;
		ar& mouseY;
		ar& mouseInput;
		ar& keyInput;

		ar& bossHitCount;
		ar& playerHitCount;
	}
};

class socketManager
{
public:
	socketManager();
	~socketManager();

	int Initialize();
	bool Shutdown();
	
	bool Frame(bool,playerInfo);
	playerInfo* GetNewMessage();

public:
	int playerId;
	playerInfo pInfo;
	std::queue<playerInfo*> serverReadBuffer;

private:
	void ListenToServer();
	playerInfo* receiveMessage(SOCKET);
	int sendMessage(SOCKET, playerInfo);
	void CopyPlayerInfo(playerInfo*, playerInfo*);

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