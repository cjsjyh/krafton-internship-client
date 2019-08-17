#include <queue>

#include "socketInterface.h"

int socketInterface::frame = 0;

int socketInterface::playerId = -1;
float socketInterface::curPlayerDirVec[3] = { 0, };
float socketInterface::curPlayerPos[3] = { 0, }; // Myself
float socketInterface::playerPos[2][3] = { 0, };
int socketInterface::bossHitCount = 0; // Myself
int socketInterface::playerHitCount = 0; // Myself
int socketInterface::playerHeal[2] = { 0, };
int socketInterface::bossHeal = 0;
int socketInterface::curPlayerUltiGauge = 0;
int socketInterface::playerUltiGauge[2] = { 0, };
bool socketInterface::UltiUsed = false;

bool socketInterface::mouseInput[][3] = { 0, };
float socketInterface::mouseDirVec[][3] = { 0, };
int socketInterface::keyInput[2][10] = { 0, };
int socketInterface::playerHp[2] = { -1, };
int socketInterface::playerMaxHp = -1;
int socketInterface::bossHp = -1;
int socketInterface::bossMaxHp = -1;
int socketInterface::bossPhase2Hp = 0;
int socketInterface::bossPhase3Hp = 0;


std::queue<int> socketInterface::bossPatternQueue;
float socketInterface::bossPos[3] = { 0, };
int socketInterface::patternId=0;
float socketInterface::patternAngle = 0.0f;
int socketInterface::patternDirCount =0;
float socketInterface::patternSrc[3] = { 0, };
float socketInterface::patternDest[3] = { 0, };
