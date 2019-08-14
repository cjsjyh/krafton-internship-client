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
	iResult = getaddrinfo("10.99.1.93", DEFAULT_PORT, &hints, &result);
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
	socketInterface::playerId = std::stoi(recvBuffer);

	//sendMessage(ConnectSocket, new hpInfo(socketInterface::playerId, socketInterface::bossHitCount, socketInterface::playerHitCount), HP_INFO);
	MsgBundle* tempMsg = receiveMessage(ConnectSocket);
	socketInterface::bossPhase2Hp = ((InitialParamBundle*)(tempMsg->ptr))->bossPhase2Hp;
	socketInterface::bossPhase3Hp = ((InitialParamBundle*)(tempMsg->ptr))->bossPhase3Hp;
	socketInterface::playerMaxHp = ((InitialParamBundle*)(tempMsg->ptr))->playerMaxHp;
	socketInterface::bossMaxHp = ((InitialParamBundle*)(tempMsg->ptr))->bossMaxHp;
	socketInterface::bossHp = socketInterface::bossMaxHp;
	for (int i = 0; i < 2; i++)
		socketInterface::playerHp[i] = socketInterface::playerMaxHp;
	delete tempMsg;

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

bool socketManager::Frame(bool IsKeyChanged, playerInput* playerInput)
{
	int iResult = 0;
	bool flag;
	if (IsKeyChanged)
	{
		iResult = sendMessage(ConnectSocket, playerInput, PLAYER_INFO);
	}

	//맞은 사람/보스가 있을 경우
	if (socketInterface::bossHitCount + socketInterface::playerHitCount > 0)
	{
		iResult = sendMessage(ConnectSocket, new hpInfo(socketInterface::playerId, socketInterface::bossHitCount, socketInterface::playerHitCount), HP_INFO);
		socketInterface::bossHitCount = 0;
		socketInterface::playerHitCount = 0;
	}

	//힐을 한 경우
	flag = false;
	for (int i = 0; i < 2; i++)
		if (socketInterface::playerHeal[i] > 0)
			flag = true;
	if (flag || socketInterface::bossHeal > 0)
	{
		hpInfo* tempHp;
		tempHp = new hpInfo;
		tempHp->bossHeal = socketInterface::bossHeal;
		for(int i=0; i<2; i++)
			tempHp->playerHeal[i] = socketInterface::playerHeal[i];
		iResult = sendMessage(ConnectSocket, tempHp, HP_INFO);

		socketInterface::bossHeal = 0;
		for(int i=0; i<2; i++)
			socketInterface::playerHeal[i] = 0;
	}

	return true;
}

void socketManager::ListenToServer()
{
	bool flag = true;
	while (flag)
	{
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
		MsgBundle* msgBundle = new MsgBundle;

		std::stringstream ss;
		ss.write(recvBuffer, msgLen);
		boost::archive::text_iarchive ia(ss);

		playerInput pInfo;
		hpInfo hpMsg;
		InitialParamBundle paramInfo;
		BossInfo bInfo;
		switch (msgType)
		{
		case PLAYER_INFO:
			ia >> pInfo;
			playerInput* pInfoPtr;
			pInfoPtr = new playerInput;
			CopyPlayerInfo(pInfoPtr, &pInfo);
			msgBundle->ptr = pInfoPtr;
			break;

		case BOSS_INFO:
			ia >> bInfo;
			BossInfo* bInfoPtr;
			bInfoPtr = new BossInfo;
			CopyBossInfo(bInfoPtr, &bInfo);
			msgBundle->ptr = bInfoPtr;
			break;

		case HP_INFO:
			ia >> hpMsg;
			hpInfo* hpInfoPtr;
			hpInfoPtr = new hpInfo;
			CopyHpInfo(hpInfoPtr, &hpMsg);
			msgBundle->ptr = hpInfoPtr;
			break;

		case ITEM_INFO:
			
			break;

		case PARAM_INFO:
			ia >> paramInfo;
			InitialParamBundle* paramInfoPtr;
			paramInfoPtr = new InitialParamBundle;
			CopyInitialParamBundle(paramInfoPtr, &paramInfo);

			msgBundle->ptr = paramInfoPtr;
			break;
		}
		msgBundle->type = msgType;
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

	playerInput pinput;
	hpInfo hInfo;
	InitialParamBundle paramInfo;
	BossInfo bInfo;
	switch (type)
	{
	case PLAYER_INFO:
		CopyPlayerInfo(&pinput, (playerInput*)_input);
		oa << pinput;
		break;
	case BOSS_INFO:
		CopyBossInfo(&bInfo, (BossInfo*)_input);
		oa << &bInfo;
		break;
	case ITEM_INFO:

		break;
	case HP_INFO:
		CopyHpInfo(&hInfo, (hpInfo*)_input);
		oa << hInfo;
		break;
	case PARAM_INFO:
		CopyInitialParamBundle(&paramInfo, (InitialParamBundle*)_input);
		oa << paramInfo;
		break;
	}

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

void socketManager::CopyPlayerInfo(playerInput* dest, playerInput* src)
{
	for (int i = 0; i < sizeof(src->keyInput) / sizeof(int); i++)
		dest->keyInput[i] = src->keyInput[i];
	for (int i = 0; i < sizeof(src->mouseInput); i++)
		dest->mouseInput[i] = src->mouseInput[i];
	for (int i = 0; i < 3; i++)
	{
		dest->playerPos[i] = src->playerPos[i];
		dest->mouseDirVec[i] = src->mouseDirVec[i];
	}
	
	dest->playerId = src->playerId;
}

void socketManager::CopyHpInfo(hpInfo* dest, hpInfo* src)
{
	dest->playerId = src->playerId;
	dest->bossHitCount = src->bossHitCount;
	dest->bossHp = src->bossHp;
	dest->playerHitCount = src->playerHitCount;
	for (int i = 0; i < sizeof(src->playerHp) / sizeof(int); i++)
	{
		dest->playerHp[i] = src->playerHp[i];
		dest->playerHeal[i] = src->playerHeal[i];
	}
	dest->bossHeal = src->bossHeal;
}

void socketManager::CopyInitialParamBundle(InitialParamBundle* dest, InitialParamBundle* src)
{
	dest->bossMaxHp = src->bossMaxHp;
	dest->bossPhase2Hp = src->bossPhase2Hp;
	dest->bossPhase3Hp = src->bossPhase3Hp;
	dest->playerMaxHp = src->playerMaxHp;
}

void socketManager::CopyItemInfo(ItemInfo* dest, ItemInfo* src)
{
	dest->itemId = src->itemId;
	dest->playerId = src->itemId;
}

void socketManager::CopyBossInfo(BossInfo* dest, BossInfo* src)
{
	dest->patternId = src->patternId;
}
