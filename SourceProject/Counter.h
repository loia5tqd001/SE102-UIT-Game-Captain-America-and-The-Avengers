#pragma once 
#include <chrono>

class Counter
{
private:
	float timePass = 0;
	std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	void Counting()
	{
		last = now;
		now = std::chrono::steady_clock::now();
		std::chrono::duration<float> duration = now - last;
		timePass += duration.count();
	}
public:
	Counter() = default;
	bool CanExcuteCommand(float time, bool resetCounter)
	{
		if (resetCounter) timePass = 0;
		Counting();
		if (timePass >= time)
		{
			timePass = 0;
			return true;
		}
		return false;
	}
};

