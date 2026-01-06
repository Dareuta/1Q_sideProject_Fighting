//======================================================================
// MenuScene.cpp
//======================================================================
#include "framework.hpp"
#include "MenuScene.hpp"
#include "../GDIEngineLib/SceneManager.hpp"
#include "../GDIEngineLib/InputManager.hpp"
#include "../GDIEngineLib/RenderManager.hpp"
#include "../GDIEngineLib/WinGameApp.hpp"

void MenuScene::Enter() {
    // 초기화 필요 시 여기에
}

void MenuScene::Update() {
    if (InputManager::Get().IsKeyPressed(VK_RETURN)) {
        SceneManager::Get().ChangeScene(1); // 게임 시작
    }
}

void MenuScene::Exit() {
    // 종료 처리 필요 시 여기에
}

void MenuScene::Render() {
     int centerX = WinGameApp::Get().GetWidth() / 2;

    int centerY = WinGameApp::Get().GetHeight() / 2;

    std::wstring title = L"WINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECTWINDOWGDIPROJECT";
    std::wstring startPrompt = L"Press Enter to Start";

    RenderManager::Get().DrawTextSimple(title, 0, centerY - 60, Gdiplus::Color::Red);
    RenderManager::Get().DrawTextSimple(startPrompt, centerX - 150, centerY + 20, Gdiplus::Color::White);
}
