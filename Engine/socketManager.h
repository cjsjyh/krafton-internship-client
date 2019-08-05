#pragma once
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
using namespace boost::iostreams;

#define BUFFER_SIZE 512

class playerInfo
{

public:
	friend class boost::serialization::access;

	playerInfo(int _playerID, int _mouseX, int _mouseY, bool* _mouseInput, int* _keyInput)
		:playerID(_playerID), mouseX(_mouseX), mouseY(_mouseY)
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
	int playerID;
	int mouseX;
	int mouseY;

	bool mouseInput[3];
	int keyInput[10];

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerID;
		ar& mouseX;
		ar& mouseY;
		ar& mouseInput;
		ar& keyInput;
	}
};

class socketManager
{
public:
	socketManager();
	~socketManager();

	bool Shutdown();
	bool Frame(bool);

	playerInfo pInfo;
	int testete;
private:
	int Initialize();
	int receiveMessage(SOCKET);
	int sendMessage(SOCKET);

	char sendBuffer[BUFFER_SIZE];
	char recvBuffer[BUFFER_SIZE];
	std::vector<int> delimiterIndex;

	SOCKET ConnectSocket;
	int count=0;
};




#endif