#include "socketInterface.h"

int socketInterface::playerId = -1;
float socketInterface::playerPos[3] = { 0, }; // Myself
int socketInterface::bossHitCount = 0; // Myself
int socketInterface::playerHitCount = 0; // Myself

bool socketInterface::mouseInput[][3] = { 0, };
int socketInterface::mouseX[2] = { 0, };
int socketInterface::mouseY[2] = { 0, };
int socketInterface::keyInput[2][10] = { 0, };
int socketInterface::playerHp[2] = { 10, };
int socketInterface::playerMaxHp = 10;
int socketInterface::bossHp = 10;
int socketInterface::bossMaxHp = 10;
int socketInterface::bossPhase2Hp = 0;
int socketInterface::bossPhase3Hp = 0;


float socketInterface::bossPos[3] = { 0, };
int socketInterface::patternId=0;
float socketInterface::patternAngle =0.0f;
int socketInterface::patternDirCount =0;
float socketInterface::patternSrc[3] = { 0, };
float socketInterface::patternDest[3] = { 0, };
