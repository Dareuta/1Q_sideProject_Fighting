//======================================================================
//PlayScene.hpp
//======================================================================
#pragma once
#include "../GDIEngineLib/Scene.hpp"
class StaticObject;
class Player;

class PlayScene :
	public Scene
{
public:
	PlayScene() = default;
	~PlayScene() = default;

	void Enter() override;
	void Exit() override;
	void Update() override;

	StaticObject* GetLeftWall() const { return pLeftWall; }
	StaticObject* GetRightWall() const { return pRightWall; }
private:
	StaticObject* pLeftWall = nullptr;
	StaticObject* pRightWall = nullptr;
	StaticObject* pGround = nullptr;

	Player* pPlayer1 = nullptr;
	Player* pPlayer2 = nullptr;

};