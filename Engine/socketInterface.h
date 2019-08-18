#pragma once
#ifndef _SOCKETINTERFACE_H_
#define _SOCKETINTERFACE_H_

class socketInterface
{
	
public:

	static int frame;
	//player
	//To Send
	static int playerId;
	static float curPlayerDirVec[3];
	static float curPlayerPos[3]; // Myself
	static int bossHitCount; // Myself
	static int playerHitCount; // Myself
	static int playerHeal[2];
	static int bossHeal;
	static int curPlayerUltiGauge;
	static bool UltiUsed;

	//To read
	static float mouseDirVec[2][3];
	static bool mouseInput[2][3];
	static int keyInput[2][10];

	static float playerPos[2][3]; // Myself
	static int playerMaxHp;
	static int playerHp[2];
	static int playerAttackType[2];
	static int playerPassiveType[2];
	static int playerUltiGauge[2];

	static std::queue<int> bossPatternQueue;
	static std::queue<int> bossPatternFrame;
	static std::queue<int> bossPatternTarget;
	static int bossHp;
	static int bossMaxHp;
	static int bossPhase2Hp;
	static int bossPhase3Hp;
	
	//boss
	static float bossPos[3];
	static int patternId;
	static float patternAngle;
	static int patternDirCount;
	static float patternSrc[3];
	static float patternDest[3];
};

#endif