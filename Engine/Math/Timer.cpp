#include "Timer.h"

void Timer::Initialize()
{
	nowSecond = 0;
	nowWaveSecond = 0;
	bossSecond = 0;
	nowFrame = 0;
	nowWaveFrame = 0;
	bossFrame = 0;
}

Timer::~Timer()
{
}
