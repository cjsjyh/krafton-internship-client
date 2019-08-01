#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
using namespace boost::iostreams;

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "playerInfo.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "socketManager.h"

class tst {
public:
	friend class boost::serialization::access;

	tst(std::string sName, int sage, float spi)
		:Name(sName), age(sage), pi(spi)
	{}

	tst() {
	}

	~tst() {

	}

	std::string Name;
	int age;
	float pi;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& Name;
		ar& age;
		ar& pi;
	}


};


socketManager::socketManager()
{
	Initialize();
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

bool socketManager::Frame()
{
	int iResult;
	// Receive until the peer shuts down the connection
	//sendMessage(ConnectSocket);
	
	iResult = receiveMessage(ConnectSocket);
	
	if (iResult > 0) {
		std::cout << "Message Received: " + std::to_string(iResult) + " Bytes" << std::endl;
		//sendMessage(ConnectSocket);
	}

	else if (iResult == 0)
		printf("Connection closing...\n");
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
		break;
	}

	//Validate Socket
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	return 0;
}

int socketManager::receiveMessage(SOCKET ConnectSocket)
{
	int iResult;
	memset(recvBuffer, 0, sizeof(recvBuffer));
	iResult = recv(ConnectSocket, recvBuffer, BUFFER_SIZE, 0); // returns number of bytes received or error
	if (iResult > 0)
	{
		//FIND \n INDEX
		delimiterIndex.clear();
		for (int i = 0; i < strlen(recvBuffer); i++)
			if (recvBuffer[i] == '\n')
				delimiterIndex.push_back(i);

		//HANDLE EACH MESSAGE BY DELIMITER
		int prevEnd = -1;
		for (int i=0; i<delimiterIndex.size(); i++)
		{
			int messageLen = delimiterIndex[i];
			std::stringstream ss;
			ss.write(&(recvBuffer[prevEnd+1]), delimiterIndex[i] - (prevEnd+1));
			boost::archive::text_iarchive ia(ss);
			prevEnd = delimiterIndex[i] + 1;
			playerInfo pInfo;
			ia >> pInfo;

			std::cout << "ID: " << pInfo.playerID << std::endl;
			std::cout << "mouseX: " << pInfo.mouseX << std::endl;
			std::cout << "mouseY: " << pInfo.mouseY << std::endl << std::endl;
		}
		//DESERIALIZATION FROM CHAR*
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	return iResult;
}

bool socketManager::sendMessage(SOCKET ClientSocket)
{
	int iSendResult;
	memset(sendBuffer, 0, sizeof(sendBuffer));

	array_sink sink{ sendBuffer };
	stream<array_sink> os{ sink };

	tst T("Tab", 31, 3.1415);
	boost::archive::text_oarchive oa(os);
	oa << T;

	sendBuffer[strlen(sendBuffer)] = '\n';

	iSendResult = send(ClientSocket, sendBuffer, strlen(sendBuffer), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		ClientSocket = 0;
		return false;
	}
	printf("Bytes sent: %d\n", iSendResult);

	return true;
}