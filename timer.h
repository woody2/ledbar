#include "memory.h"

#define TIMER_BASE      (BCM2708_BASE + 0x003000)       // 0x7e003000

enum kTimerWordOffsets {
	kTimerCS      = 0,
	kTimerCLO     = 1,
	kTimerCHI     = 2,
	kTimerC0      = 3,
	kTimerC1      = 4,
	kTimerC2      = 5,
	kTimerC3      = 6
};

volatile unsigned long *mTimer;
int mLastResetTime;

void initTimer();

void ResetStartTime();

void Wait(int elapsedtime);
