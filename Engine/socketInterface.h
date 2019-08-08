#pragma once
#ifndef _SOCKETINTERFACE_H_
#define _SOCKETINTERFACE_H_

class socketInterface
{
	
public:
	//player
	//To Send
	static int clientId;
	static float playerPos[3]; // Myself
	static int bossHitCount; // Myself
	static int playerHitCount; // Myself

	//To read
	static int mouseX[2], mouseY[2];
	static bool mouseInput[2][3];
	static int keyInput[2][10];
	
	//boss
	static float bossPos[3];
	static int patternId;
	static float patternAngle;
	static int patternDirCount;
	static float patternSrc[3];
	static float patternDest[3];
};

#endif