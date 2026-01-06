//======================================================================
//TimeManager.cpp
//======================================================================
#include "pch.hpp"
#include "TimeManager.hpp"

//freq, start, previous, current

void TimeManager::StartUp()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	previous = start;
	delta = 0.0f;
}

void TimeManager::Shutdown()
{

}

void TimeManager::Update()
{
	QueryPerformanceCounter(&current);
	delta = static_cast<float>(current.QuadPart - previous.QuadPart) / static_cast<float>(freq.QuadPart);
	previous = current;
}

float TimeManager::Elapsed() {
	return static_cast<float>(current.QuadPart - start.QuadPart) / static_cast<float>(freq.QuadPart);
}