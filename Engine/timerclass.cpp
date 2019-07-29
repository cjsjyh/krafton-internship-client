#include "stdafx.h"

#include "timerclass.h"

timerclass::timerclass()
{

}

timerclass::~timerclass()
{

}

void timerclass::Frame()
{
	frame++;

	if (frame < triggerFrame)
	{
		*sourceFloat += amountFloat;
	}
}

void timerclass::SetTimerGradualIncrease(float* source, float amount, int duration)
{
	sourceFloat = source;
	triggerFrame = frame + duration;
	amountFloat = amount / duration;
}