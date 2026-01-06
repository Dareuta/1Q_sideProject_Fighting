//======================================================================
//TimeManager.hpp
//======================================================================
#pragma once
#include "Singleton.hpp"

class TimeManager : public Singleton<TimeManager>
{
private:
	LARGE_INTEGER freq, start, previous, current;
	float delta;
public:
	TimeManager() = default;
	~TimeManager() = default;
	void StartUp();
	void Shutdown();
	void Update();

	float Elapsed();
	float GetDelta() const {
		return delta;
	}
};
