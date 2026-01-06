//======================================================================
//InputManager.hpp
//======================================================================
#pragma once
#include "Singleton.hpp"

class InputManager : public Singleton<InputManager>
{
public:
	InputManager() = default;
	~InputManager() = default;

	HWND m_hWnd; // 윈도우 핸들
	POINT m_mouseClient;	// 마우스 좌표
	SHORT m_prevState[256] = { 0 };
	SHORT m_currState[256] = { 0 };
public:
	void Startup(HWND hWnd);
	void Shutdown() {};

	void Update();

	bool IsKeyDown(int vKey);
	bool IsKeyPressed(int vKey);
	bool IsKeyReleased(int vKey);
};