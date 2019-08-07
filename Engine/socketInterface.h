#pragma once
#ifndef _SOCKETINTERFACE_H_
#define _SOCKETINTERFACE_H_

class socketInterface
{
	//player
public:
	static int playerId;
	static int playerPos_x, playerPos_y, playerPos_z;
	static int mouseX, mouseY;
	static bool mouseInput[3];
	static int keyInput[10];
	static int bossHitCount;
	static int playerHitCount;

	//boss
	static int bossPos_x, bossPos_y, bossPos_z;
	static int patternId;
	static float patternAngle;
	static int patternDirCount;
	static int patternSrc_x, patternSrc_y, patternSrc_z;
	static int patternDest_x, patternDest_y, patternDest_z;

	socketInterface();
	~socketInterface();
};

#endif