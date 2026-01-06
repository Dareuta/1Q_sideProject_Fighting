//======================================================================
//PlayScene.cpp
//======================================================================
#include "framework.hpp"
#include "PlayScene.hpp"
#include "Player.hpp"
#include "StaticObject.hpp"
#include "BackGround.hpp"



void PlayScene::Enter()
{
	__super::Enter(); // Scene::Enter();

	//벽 먼저
	pLeftWall = CreateObject<StaticObject>(0.0f, 0.0f, 50, 1080, StaticObjectType::Wall);
	pRightWall = CreateObject<StaticObject>(1920.0f - 50.0f, 0.0f, 50, 1080, StaticObjectType::Wall);
	pGround = CreateObject<StaticObject>(0, 1000, 3072, 100, StaticObjectType::Ground);
	// 플레이어
	pPlayer1 = (Player*)CreateObject<Player>(ControlType::Player1);
	pPlayer1->SetLayer(2);
	pPlayer1->SetWall(pLeftWall, pRightWall, pGround);

	pPlayer2 = (Player*)CreateObject<Player>(ControlType::Player2);
	pPlayer2->SetLayer(2);
	pPlayer2->SetWall(pLeftWall, pRightWall, pGround);

	pPlayer1->SetEnemy(pPlayer2);
	pPlayer2->SetEnemy(pPlayer1);

	pPlayer1->SetPosition({ 1056,500 });
	pPlayer2->SetPosition({ 2016,500 });


	// 배경
	Background* pBackground1 = CreateObject<Background>(0.0f, 0.0f, L"BackGround", 3072, 2048);
	pBackground1->SetPlayers(pPlayer1, pPlayer2);
	pBackground1->SetLayer(-1);

	/*
	//pBackground1->SetParallaxFactor(1.0f); //패럴랙스는 안하는게 맞을듯
	//pBackground1->SetIndex(0);

	// 992 614 0.26
	// 1252 763 0.33
	// 1520 654 0.4

	Background* pBackground2 = CreateObject<Background>(0.0f, 0.0f, L"BackGround", 3840, 1080);
	pBackground2->SetPlayers(pPlayer1, pPlayer2);
	pBackground2->SetLayer(-2);
	pBackground2->SetParallaxFactor(0.33f);
	pBackground2->SetIndex(1);

	Background* pBackground3 = CreateObject<Background>(0.0f, 0.0f, L"BackGround", 3840, 1080);
	pBackground3->SetPlayers(pPlayer1, pPlayer2);
	pBackground3->SetLayer(-1);
	pBackground3->SetParallaxFactor(0.40f);
	pBackground3->SetIndex(2);
	*/

}

void PlayScene::Exit()
{
	// 뭔가를 한다.
	__super::Exit();
}

void PlayScene::Update()
{
	Scene::Update();

	// PlayScene::Update
	int cameraX = RenderManager::Get().GetCameraOffsetX();
	int screenW = WinGameApp::Get().GetWidth();

	if (pLeftWall)
		pLeftWall->SetPos(Vector2(cameraX - 50.0f, 0.0f)); // 왼쪽은 카메라 왼쪽 외곽

	if (pRightWall)
		pRightWall->SetPos(Vector2(cameraX + static_cast<float>(screenW), 0.0f)); // 오른쪽은 카메라 오른쪽 외곽





	if (pPlayer1->GetHP() <= 0 || pPlayer2->GetHP() <= 0) {
		int winner = (pPlayer1->GetHP() > 0) ? 1 : 2;
		SceneManager::Get().SetParam(winner); // 승자 번호 넘겨주기
		SceneManager::Get().ChangeScene(2);
	}


	//if (pGround)
	//	pGround->SetPos(0.0f, 1000.0f - cameraY); //나중에 구현하자

	// Play하는 Scene은 충돌처리를 더 추가한다.
}
