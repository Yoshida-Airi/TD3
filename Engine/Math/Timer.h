#pragma once
#include<stdint.h>
class Timer
{
private:
	int nowSecond;
	int nowWaveSecond;
	int bossSecond;
	int nowFrame;
	int nowWaveFrame;
	int bossFrame;

public:
	~Timer();
	void Initialize();
	int GetNowSecond() { return nowSecond; }
	int GetNowWaveSecond() { return nowWaveSecond; }
	int GetBossSecond() { return bossSecond; }
	int GetNowFrame() { return nowFrame; }
	int GetNowWaveFrame() { return nowWaveFrame; }
	int GetBossFrame() { return bossFrame; }
	
	void AddNowSecond() { nowSecond++; }
	void AddNowWaveSecond() { nowWaveSecond++; }
	void AddBossSecond() { bossSecond++; }
	void AddNowFrame() { nowFrame++; }
	void AddNowWaveFrame() { nowWaveFrame++; }
	void AddBossFrame() { bossFrame++; }
	
	void ResetNowSecond() { nowSecond = 0; }
	void ResetNowWaveSecond() { nowWaveSecond = 0; }
	void ResetBossSecond() { bossSecond = 0; }
	void ResetNowFrame() { nowFrame = 0; }
	void ResetNowWaveFrame() { nowWaveFrame = 0; }
	void ResetBossFrame() { bossFrame = 0; }
};

