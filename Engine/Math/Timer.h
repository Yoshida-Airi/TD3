#pragma once
#include<stdint.h>

class Timer
{
private:
	uint32_t nowSecond;
	uint32_t nowWaveSecond;
	uint32_t bossSecond;
	uint32_t nowFrame;
	uint32_t nowWaveFrame;
	uint32_t bossFrame;

public:
	~Timer();
	void Initialize();
	uint32_t GetNowSecond() { return nowSecond; }
	uint32_t GetNowWaveSecond() { return nowWaveSecond; }
	uint32_t GetBossSecond() { return bossSecond; }
	uint32_t GetNowFrame() { return nowFrame; }
	uint32_t GetNowWaveFrame() { return nowWaveFrame; }
	uint32_t GetBossFrame() { return bossFrame; }
	
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

