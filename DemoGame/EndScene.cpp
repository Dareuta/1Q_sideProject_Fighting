#include "framework.hpp"
#include "EndScene.hpp"
#include "../GDIEngineLib/SceneManager.hpp"
#include "../GDIEngineLib/InputManager.hpp"
#include "../GDIEngineLib/RenderManager.hpp"
#include "../GDIEngineLib/WinGameApp.hpp"

int winner = 0;

void EndScene::Enter()
{
	winner = SceneManager::Get().GetParam(); // 승자 번호 가져오기
}

void EndScene::Update()
{
	if (InputManager::Get().IsKeyPressed(VK_RETURN)) {
		SceneManager::Get().ChangeScene(0); // 메뉴로 돌아가기
	}
}

void EndScene::Render() {
    std::wstring winMsg = (winner == 1) ? L"Player 1 Wins!" : L"Player 2 Wins!";
    std::wstring retryMsg = L"Press Enter to Return to Menu";

    int centerX = WinGameApp::Get().GetWidth() / 2;
    int centerY = WinGameApp::Get().GetHeight() / 2;

    RenderManager::Get().DrawTextSimple(winMsg, centerX - 100, centerY - 50, Gdiplus::Color::White);
    RenderManager::Get().DrawTextSimple(retryMsg, centerX - 150, centerY + 20, Gdiplus::Color::Yellow);
}


void EndScene::Exit()
{
	// 필요시 리소스 초기화
}
