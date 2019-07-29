#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

class timerclass
{
public:
	timerclass();
	~timerclass();

	bool Frame();

	void SetTimerGradualChange(float* source, float amount, int duration);

private:
	int frame;
	int triggerFrame;

	float* sourceFloat;
	float amountFloat;
};

#endif
