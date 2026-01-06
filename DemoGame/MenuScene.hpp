//======================================================================
//MenuScene.hpp
//======================================================================
#pragma once
#include "../GDIEngineLib/Scene.hpp"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void Render();
	void Update() override;
	void Enter() override;
	void Exit() override;
};

