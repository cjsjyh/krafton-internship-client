#include "stdafx.h"

#include "timerclass.h"

timerclass::timerclass()
{

}

timerclass::~timerclass()
{

}

bool timerclass::Frame()
{
	frame++;

	if (frame < triggerFrame)
	{
		*sourceFloat += amountFloat;
		return false;
	}
	else {
		*sourceFloat += amountFloat;
		return true;
	}
}

void timerclass::SetTimerGradualChange(float* source, float amount, int duration)
{
	sourceFloat = source;
	triggerFrame = frame + duration;
	amountFloat = amount / duration;
}