//======================================================================
//EndScene.hpp
//======================================================================
#pragma once
#include "../GDIEngineLib/Scene.hpp"

class EndScene :
	public Scene
{
public:
	EndScene() = default;
	~EndScene() = default;

	void Render();
	void Update() override;
	void Enter() override;
	void Exit() override;
};

