//======================================================================
//DemoGameApp.hpp
//======================================================================
#pragma once
#include "../GDIEngineLib/WinGameApp.hpp"

class DemoGameApp : public WinGameApp {
public:
	DemoGameApp() = default;
	~DemoGameApp() = default;

	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void LoadAllPng();
	void LoadMayPng();
	void LoadBackGroundPng();
};
 