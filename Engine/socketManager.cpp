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


socketManager::socketManager()
{
	//m_Input = 0;
}

socketManager::~socketManager()
{

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

bool socketManager::Frame(bool IsKeyChanged,playerInfo playerInput)
{
	int iResult = 0 ;
	// Receive until the peer shuts down the connection
	//sendMessage(ConnectSocket);
	
	if (IsKeyChanged)
	{
		std::cout << "Key changed!" << std::endl;
		iResult = sendMessage(ConnectSocket, playerInput);
		iResult = receiveMessage(ConnectSocket);
	}
	else {
		std::cout << "Key Same!" << std::endl;
	}

	if (iResult > 0) {
		std::cout << "Message Received: " + std::to_string(iResult) + " Bytes" << std::endl;
		//sendMessage(ConnectSocket);
	}

	else if (iResult == 0)
		printf("Nothing received...\n");
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	return true;
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
	std::cout << "Client ID: " + std::to_string(playerId) << std::endl;
	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		playerInfo tempPlayer;
		playerInput.push_back(tempPlayer);
	}

	return 0;
}

int socketManager::receiveMessage(SOCKET ConnectSocket)
{
	int iResult;
	//First receive size of data that needs to be read
	memset(recvBuffer, 0, sizeof(recvBuffer));
	iResult = recv(ConnectSocket, recvBuffer, sizeof(int), 0);
	if (iResult > 0)
	{
		std::cout << "count: " + std::to_string(count++) << std::endl;
		//read to know how many bytes to receive
		int msgLen = std::stoi(recvBuffer);
		memset(recvBuffer, 0, sizeof(recvBuffer));
		//read real messages
		iResult = recv(ConnectSocket, recvBuffer, msgLen, 0); // returns number of bytes received or error
		if (iResult > 0)
		{
			//FIND \n INDEX
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
				ss.write(&(recvBuffer[prevEnd + 1]), delimiterIndex[i] - (prevEnd + 1));
				boost::archive::text_iarchive ia(ss);
				prevEnd = delimiterIndex[i] + 1;

				//if (pInfo)
				//	delete pInfo;
				//pInfo = new playerInfo();
				//playerInfo pInfo;
				ia >> pInfo;
				
				playerInput[pInfo.playerId].mouseX = pInfo.mouseX;
				playerInput[pInfo.playerId].mouseY = pInfo.mouseY;
				for(int i=0;i<sizeof(pInfo.keyInput)/sizeof(int);i++)
					playerInput[pInfo.playerId].keyInput[i] = pInfo.keyInput[i];
				for(int i=0;i<sizeof(pInfo.mouseInput);i++)
					playerInput[pInfo.playerId].mouseInput[i] = pInfo.mouseInput[i];
			}
		}
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	return iResult;
}

int socketManager::sendMessage(SOCKET ClientSocket, playerInfo input)
{
	int iSendResult;
	memset(sendBuffer, 0, sizeof(sendBuffer));

	array_sink sink{ sendBuffer };
	stream<array_sink> os{ sink };

	/*bool tempBool[3];
	for (int i = 0; i < 3; i++)
		tempBool[i] = true;
	int tempInt[10];
	tempInt[0] = 0x11;*/
	//playerInfo T(playerId, 100, 100, tempBool, tempInt);


	boost::archive::text_oarchive oa(os);
	oa << input;
	sendBuffer[strlen(sendBuffer)] = '\n';

	std::string msgLen = std::to_string(strlen(sendBuffer));
	const char* msgLenChar = msgLen.c_str();
	iSendResult = send(ClientSocket, msgLenChar, sizeof(int), 0);

	iSendResult = send(ClientSocket, sendBuffer, strlen(sendBuffer), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return -1;
	}

	std::cout << sendBuffer << std::endl;
	printf("Bytes sent: %d\n", iSendResult);

	return iSendResult;
}


