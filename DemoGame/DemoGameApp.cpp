//======================================================================
//DemoGameApp.cpp
//======================================================================
#include "framework.hpp"
#include "DemoGameApp.hpp"
#include "MenuScene.hpp"
#include "PlayScene.hpp"
#include "EndScene.hpp"

#include "../GDIEngineLib/SceneManager.hpp"
#include "../GDIEngineLib/ResourceManager.hpp"

void DemoGameApp::Initialize()
{
	__super::Initialize(); //  WinGameApp::Initialize();
	LoadAllPng();

	SceneManager::Get().CreateScene<MenuScene>(); // 씬 생성
	SceneManager::Get().CreateScene<PlayScene>(); // 씬 생성
	SceneManager::Get().CreateScene<EndScene>();
	SceneManager::Get().ChangeScene(0); // 씬 변경
}

void DemoGameApp::Shutdown()
{
	__super::Shutdown(); //  WinGameApp::Shutdown();
}

void DemoGameApp::LoadAllPng() {
	LoadMayPng();
	LoadBackGroundPng();
}

//메세지 받아서 처리하는데, GameAPP에서만 할꺼 받는곳 ////////////////////////////////////////////////
void DemoGameApp::MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	__super::MessageProc(hwnd, uMsg, wParam, lParam); // WM_DESTROY 기본처리

	// DemoGameApp에서만 처리하고싶은 내용
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadAndOffset(const std::wstring& folder, const std::wstring& pattern, int offsetX = 0, int offsetY = 0, float scale = 1.0f) {
	ResourceManager::Get().LoadPng(folder, pattern);

	// offset도 스케일링
	int scaledOffsetX = static_cast<int>(offsetX * scale);
	int scaledOffsetY = static_cast<int>(offsetY * scale);
	RenderManager::Get().SetOffset(pattern, scaledOffsetX, scaledOffsetY);

	// 리사이즈 처리
	if (scale != 1.0f) {
		std::vector<Gdiplus::Bitmap*>& bitmaps = RenderManager::Get().GetBitmaps(pattern);
		if (!bitmaps.empty()) {
			Gdiplus::Bitmap* original = bitmaps[0];
			int newW = static_cast<int>(original->GetWidth() * scale);
			int newH = static_cast<int>(original->GetHeight() * scale);
			RenderManager::Get().ResizeBitmapGroup(pattern, newW, newH);
		}
	}


	if (pattern.find(L"BackGround") != std::wstring::npos) {
		RenderManager::Get().ResizeBitmapGroup(pattern, 3072, 2048);
	}
	
}

void DemoGameApp::LoadMayPng() {
	float MayOffsetScale = 1.5f;

	// 지상
	LoadAndOffset(L"May\\AttackD", L"AttackD", 280, 408, MayOffsetScale);
	LoadAndOffset(L"May\\AttackH", L"AttackH", 180, 326, MayOffsetScale);
	LoadAndOffset(L"May\\AttackS", L"AttackS", 327, 274, MayOffsetScale);

	LoadAndOffset(L"May\\AttackDown", L"AttackDown", 204, 221, MayOffsetScale);
	LoadAndOffset(L"May\\AttackUp", L"AttackUp", 138, 239, MayOffsetScale);

	LoadAndOffset(L"May\\GuardDown", L"GuardDown", 93, 193, MayOffsetScale);
	LoadAndOffset(L"May\\GuardUp", L"GuardUp", 121, 252, MayOffsetScale);

	LoadAndOffset(L"May\\CrouchLoop", L"CrouchLoop", 68, 207, MayOffsetScale);
	LoadAndOffset(L"May\\NeutralOnce", L"NeutralOnce", 60, 227, MayOffsetScale);
	LoadAndOffset(L"May\\StandingLoop", L"StandingLoop", 87, 234, MayOffsetScale);

	LoadAndOffset(L"May\\BackDash", L"BackDash", 93, 245, MayOffsetScale);
	LoadAndOffset(L"May\\RunEnd", L"RunEnd", 109, 257, MayOffsetScale);
	LoadAndOffset(L"May\\RunLoop", L"RunLoop", 102, 264, MayOffsetScale);
	LoadAndOffset(L"May\\WalkbackLoop", L"WalkbackLoop", 80, 209, MayOffsetScale);
	LoadAndOffset(L"May\\WalkLoop", L"WalkLoop", 78, 214, MayOffsetScale);

	LoadAndOffset(L"May\\HitUp", L"HitUp", 145, 250, MayOffsetScale);
	LoadAndOffset(L"May\\HitNormal", L"HitNormal", 119, 232, MayOffsetScale);
	LoadAndOffset(L"May\\HitDown", L"HitDown", 145, 315, MayOffsetScale);

	// 공중
	LoadAndOffset(L"May\\HitAir", L"HitAir", 155, 281, MayOffsetScale); //160
	LoadAndOffset(L"May\\AirAttackH", L"AirAttackH", 181, 385, MayOffsetScale); //197
	LoadAndOffset(L"May\\AirAttackS", L"AirAttackS", 164, 357, MayOffsetScale); //185

	LoadAndOffset(L"May\\JumpBack", L"JumpBack", 95, 279, MayOffsetScale); //146
	LoadAndOffset(L"May\\JumpFront", L"JumpFront", 104, 299, MayOffsetScale); //100
	LoadAndOffset(L"May\\JumpNormal", L"JumpNormal", 94, 300, MayOffsetScale); //143
}


void DemoGameApp::LoadBackGroundPng() {
	LoadAndOffset(L"BackGround", L"BackGround"); // 1520 * 654? 
}