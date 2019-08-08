#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include <iostream>
#include <sstream>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#include "socketManager.h"

#include "socketInterface.h"

socketManager::socketManager()
{
	threadLock = new std::mutex;
	//m_Input = 0;
}

socketManager::~socketManager()
{

}

int socketManager::Initialize()
{
	//m_Input = _m_Input;

	WSADATA wsaData; //Contains information aout the Windows Sockets implementation.
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // initiate the use of WS2_32.dll // = Winsock version 2.2 used
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//addrinfo contains sockaddr structure
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; //either IPv6 or IPv4 address can be returned
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	ConnectSocket = INVALID_SOCKET;

	// Resolve the server address and port
	iResult = getaddrinfo("", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		// Validate Socket
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		else {
			std::cout << "socket connected!" << std::endl;
		}
		break;
	}

	//Validate Socket
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	//SET CLIENT ID
	iResult = recv(ConnectSocket, recvBuffer, sizeof(int), 0);
	playerId = std::stoi(recvBuffer);
	
	//Set Dummy for initial value
	/*MsgBundle* tempMsg = new MsgBundle;
	playerInfo* tempPlayer = new playerInfo;
	tempPlayer->playerId = 0;
	
	tempMsg->ptr = tempPlayer;
	tempMsg->type = PLAYER_INFO;
	
	serverReadBuffer.push(tempMsg);*/

	std::thread t1([&]() {ListenToServer();});
	t1.detach();

	return 0;
}

bool socketManager::Shutdown()
{
	int iResult;
	// shutdown the connection since we're done
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	return true;
}

bool socketManager::Frame(bool IsKeyChanged,playerInfo* playerInput)
{
	int iResult = 0 ;
	if (IsKeyChanged)
	{
		iResult = sendMessage(ConnectSocket, playerInput, PLAYER_INFO);
	}
	return true;
}

void socketManager::ListenToServer()
{
	bool flag = true;
	while (flag)
	{
		std::cout << "Listening" << std::endl;
		MsgBundle* tempMsg = receiveMessage(ConnectSocket);
		if (tempMsg == NULL)
		{
			printf("Receive Failed. terminating thread");
			flag = false;
		}
		else 
		{
			threadLock->lock();
			serverReadBuffer.push(tempMsg);
			threadLock->unlock();
		}
	}
}

MsgBundle* socketManager::GetNewMessage()
{
	if (serverReadBuffer.size() > 0)
	{
		threadLock->lock();
		MsgBundle* tempMsg = serverReadBuffer.front();
		serverReadBuffer.pop();
		threadLock->unlock();
		return tempMsg;
	}
	else
		return NULL;
}

MsgBundle* socketManager::receiveMessage(SOCKET ConnectSocket)
{
	int iResult;

	//Receive Type of Data
	memset(recvBuffer, 0, sizeof(recvBuffer));
	iResult = recv(ConnectSocket, recvBuffer, sizeof(int), 0);
	if (iResult <= 0)
	{
		printf("[ERROR] Recv Type Failed");
		return NULL;
	}
	int msgType = std::stoi(recvBuffer);

	//Receive Size of Data
	memset(recvBuffer, 0, sizeof(recvBuffer));
	iResult = recv(ConnectSocket, recvBuffer, sizeof(int), 0);
	if (iResult <= 0)
	{
		printf("[ERROR] Recv Len Failed");
		return NULL;
	}
	int msgLen = std::stoi(recvBuffer);

	//Receive Data
	memset(recvBuffer, 0, sizeof(recvBuffer));
	iResult = recv(ConnectSocket, recvBuffer, msgLen, 0);
	if (iResult > 0)
	{
		//FIND \n INDEX
		/*
		delimiterIndex.clear();
		for (int i = 0; i < strlen(recvBuffer); i++)
			if (recvBuffer[i] == '\n')
				delimiterIndex.push_back(i);


		//HANDLE EACH MESSAGE BY DELIMITER
		int prevEnd = -1;

		for (int i = 0; i < delimiterIndex.size(); i++)
		{
			int messageLen = delimiterIndex[i];
			std::stringstream ss;
			ss.write(&(recvBuffer[prevEnd + 1]), delimiterIndex[i] - (prevEnd + 1) );
			boost::archive::text_iarchive ia(ss);
			prevEnd = delimiterIndex[i] + 1;

			//playerInfo pInfo;
			ia >> pInfo;
			CopyPlayerInfo(pInfoPtr, &pInfo);
			std::cout << "[recv]ID: " << pInfoPtr->playerId << std::endl;
			std::cout << "[recv]mouseX: " << pInfoPtr->mouseX << std::endl;
			std::cout << "[recv]mouseY: " << pInfoPtr->mouseY << std::endl << std::endl;
		}
		*/
		std::stringstream ss;
		ss.write(recvBuffer, msgLen);
		boost::archive::text_iarchive ia(ss);

		MsgBundle* msgBundle = new MsgBundle;

		playerInfo* pInfoPtr = new playerInfo;
		playerInfo pInfo;
		switch (msgType)
		{
		case PLAYER_INFO:
			ia >> pInfo;
			CopyPlayerInfo(pInfoPtr, &pInfo);

			msgBundle->ptr = pInfoPtr;
			msgBundle->type = msgType;

			std::cout << "[recv]ID: " << pInfoPtr->playerId << std::endl;
			std::cout << "[recv]mouseX: " << pInfoPtr->mouseX << std::endl;
			std::cout << "[recv]mouseY: " << pInfoPtr->mouseY << std::endl << std::endl;
			break;
		case BOSS_INFO:

			break;
		case ITEM_INFO:

			break;
		}

		return msgBundle;
	}

	printf("[ERROR] Recv Msg Failed\n");
	return NULL;
}


int socketManager::sendMessage(SOCKET ClientSocket, void* _input, DataType type)
{
	int iSendResult;

	memset(sendBuffer, 0, sizeof(sendBuffer));
	array_sink sink{ sendBuffer };
	stream<array_sink> os{ sink };
	boost::archive::text_oarchive oa(os);

	playerInfo input;
	switch (type)
	{
	case PLAYER_INFO:
		CopyPlayerInfo(&input, (playerInfo*)_input);
		oa << input;
		break;
	case BOSS_INFO:

		break;
	case ITEM_INFO:

		break;
	}

	//sendBuffer[strlen(sendBuffer)] = '\n';

	iSendResult = 0;
	while (!iSendResult)
	{
		std::string msgType = std::to_string(type);
		const char* msgTypeChar = msgType.c_str();
		iSendResult = send(ClientSocket, msgTypeChar, sizeof(int), 0);
		if (!iSendResult)
			printf("[ERROR] Send Type Failed\n");
	}

	iSendResult = 0;
	while (!iSendResult)
	{
		std::string msgLen = std::to_string(strlen(sendBuffer));
		const char* msgLenChar = msgLen.c_str();
		iSendResult = send(ClientSocket, msgLenChar, sizeof(int), 0);
		if (!iSendResult)
			printf("[ERROR] Send Len Failed\n");
	}

	iSendResult = send(ClientSocket, sendBuffer, strlen(sendBuffer), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("[ERROR] Send Msg Failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return -1;
	}

	printf("[Success] Bytes sent: %d\n", iSendResult);
	return iSendResult;
}

void socketManager::CopyPlayerInfo(playerInfo* dest, playerInfo* src)
{
	for (int i = 0; i < sizeof(src->keyInput) / sizeof(int); i++)
		dest->keyInput[i] = src->keyInput[i];
	for (int i = 0; i < sizeof(src->mouseInput); i++)
		dest->mouseInput[i] = src->mouseInput[i];
	dest->mouseX = src->mouseX;
	dest->mouseY = src->mouseY;
	dest->playerId = src->playerId;
}