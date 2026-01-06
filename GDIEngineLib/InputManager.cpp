//======================================================================
//InputManager.cpp
//======================================================================
#include "pch.hpp"
#include "InputManager.hpp"

void InputManager::Startup(HWND hWnd)
{
	m_hWnd = hWnd; // 윈도우 핸들
}

void InputManager::Update()
{
	// 마우스 좌표
	::GetCursorPos(&m_mouseClient);
	::ScreenToClient(m_hWnd, &m_mouseClient);

	memcpy_s(m_prevState, sizeof(m_prevState), m_currState, sizeof(m_currState));
	for (int i = 0; i < 256; i++) {
		m_currState[i] = GetAsyncKeyState(i);
	}
}

bool InputManager::IsKeyDown(int vKey)
{
	return (m_currState[vKey] & 0x8000) != 0;
}

bool InputManager::IsKeyPressed(int vKey)
{
	return (!(m_prevState[vKey] & 0x8000) && (m_currState[vKey] & 0x8000));
}

bool InputManager::IsKeyReleased(int vKey)
{
	return ((m_prevState[vKey] & 0x8000) && !(m_currState[vKey] & 0x8000));
}
