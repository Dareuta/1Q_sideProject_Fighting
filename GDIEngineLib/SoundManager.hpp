//======================================================================
//SoundManager.hpp
//======================================================================
#pragma once
#include "Singleton.hpp"

class SoundManager : public Singleton<SoundManager>
{
public:
	SoundManager() = default;
	~SoundManager() = default;
	void Startup() {};
	void Shutdown() {};

	void Update() {};
};

