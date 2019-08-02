#pragma once
#ifndef _SOCKET_H_
#define _SOCKET_H_

#define BUFFER_SIZE 512

class socketManager
{
public:
	socketManager();
	~socketManager();

	bool Shutdown();
	bool Frame();
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