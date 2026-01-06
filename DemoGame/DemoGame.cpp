//======================================================================
//DemoGame.cpp
//======================================================================
// DemoGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//


#include "framework.hpp"
#include "DemoGame.hpp"
#include "DemoGameApp.hpp"
/////////////////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	DemoGameApp app;

	app.Initialize();
	app.Run();
	app.Shutdown();

	return (int)1;
}

