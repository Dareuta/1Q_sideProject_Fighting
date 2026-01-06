//======================================================================
// BackGround.hpp
//======================================================================

#pragma once
#include "framework.hpp"
#include "../GDIEngineLib/Object.hpp"
#include "../GDIEngineLib/RenderManager.hpp"
#include "../GDIEngineLib/WinGameApp.hpp"

class Background : public Object {
public:
	Background(float x, float y, const std::wstring& key, int width, int height);
	~Background() override = default;

	void Update() override;
	void Render() override;

	void SetPlayers(Object* p1, Object* p2);
	void SetParallaxFactor(float factor) { m_ParallaxFactor = factor; }
	void SetIndex(int index) { m_index = index; }

private:
	float m_ParallaxFactor = 1.0f;
	int m_index = 0;

	int pOffsetX = 0;
	int pOffsetY = 0;

	int screenWidth = 0;
	int screenHeight = 0;

	std::wstring m_ImageKey;
	Object* m_pPlayer1 = nullptr;
	Object* m_pPlayer2 = nullptr;
	int m_BackgroundWidth = 0;
	int m_BackgroundHeight = 0;
};
