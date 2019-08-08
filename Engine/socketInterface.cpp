#include "socketInterface.h"

int socketInterface::clientId = -1;
float socketInterface::playerPos[3] = { 0, }; // Myself
int socketInterface::bossHitCount = 0; // Myself
int socketInterface::playerHitCount = 0; // Myself

bool socketInterface::mouseInput[][3] = { 0, };
int socketInterface::mouseX[2] = { 0, };
int socketInterface::mouseY[2] = { 0, };
int socketInterface::keyInput[2][10] = { 0, };

float socketInterface::bossPos[3] = { 0, };
int socketInterface::patternId=0;
float socketInterface::patternAngle =0.0f;
int socketInterface::patternDirCount =0;
float socketInterface::patternSrc[3] = { 0, };
float socketInterface::patternDest[3] = { 0, };
