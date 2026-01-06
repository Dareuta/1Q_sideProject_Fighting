//======================================================================
//WinGameApp.hpp
//======================================================================

#pragma once
#include "InputManager.hpp"
#include "SceneManager.hpp"
#include "TimeManager.hpp"
#include "SoundManager.hpp"
#include "RenderManager.hpp"
#include "ResourceManager.hpp"
#include "Singleton.hpp"

#define FRAME_RATE  0.0166667f

class WinGameApp : public Singleton<WinGameApp>
{
public:
	WinGameApp();
	virtual ~WinGameApp();
	static WinGameApp* m_pInstance;
	HWND GetHWND() {
		return m_hWnd;
	}

private:
	HINSTANCE		m_hInstance;
	int				m_Width = 1920; // 넓이설정
	int				m_Height = 1080; // 높이설정
	std::wstring	m_WindowName = L"What's this"; // 윈도우 이름 << 어디뜨는거지 << 안뜬데
	std::wstring	m_TitleName = L"사랑스러운 윈도우GDI 코드읽기 쉽지않네"; // 상단에 뜨는 창 이름 
	std::string		m_ModulePath;
	std::string		m_WorkingPath;
	HWND			m_hWnd;
	float delta = 0;

	// new로 생성하면 여러 h 포함응 전방선언을 해결가능
	//이거 왜 여기서 선언하고 있냐
	InputManager    m_InputManager;
	SceneManager    m_SceneManager;
	TimeManager     m_TimeManager;
	SoundManager    m_SoundManager;
	RenderManager   m_RenderManager;
	ResourceManager m_ResourceManager;
public:
	virtual void Initialize();
	virtual void Shutdown();
	virtual void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Run();
	int GetHeight() {
		return m_Height;
	}
	int GetWidth() {
		return m_Width;
	}

private:
	void Update();
	void Render();
};

