//======================================================================
// BackGround.cpp
//======================================================================

#include "BackGround.hpp"


#undef max
#undef min

Background::Background(float x, float y, const std::wstring& key, int width, int height)
    : m_ImageKey(key), m_BackgroundWidth(width), m_BackgroundHeight(height)
{
    position = Vector2(x, y);  
    screenWidth = WinGameApp::Get().GetWidth();
    screenHeight = WinGameApp::Get().GetHeight();

    pOffsetX = RenderManager::Get().GetStandardOffsetX();
    pOffsetY = RenderManager::Get().GetStandardOffsetY();
}

void Background::Update() {
    if (!m_pPlayer1 || !m_pPlayer2)
        return;

    RECT p1Col = m_pPlayer1->GetCollider();
    RECT p2Col = m_pPlayer2->GetCollider();

    float centerX = (p1Col.right + p2Col.right + p1Col.left + p2Col.left) / 4.0f; // 두 캐릭터의 콜라이더의 중심점을 기준으로 잡은 중앙값
    float focusX = centerX - (screenWidth / 2.0f);

    if (focusX < 0) focusX = 0;
    if (focusX > m_BackgroundWidth - screenWidth)
        focusX = m_BackgroundWidth - screenWidth;

    float bottomY = std::min(p1Col.bottom, p2Col.bottom);
    float targetY = bottomY - (screenHeight / 2.0f);
    float focusY = (bottomY < screenHeight / 2.0f) ? targetY : 0.0f;

    position.x = -focusX * m_ParallaxFactor -pOffsetX;
    position.y = -m_BackgroundHeight + screenHeight - pOffsetY;

    RenderManager::Get().SetCameraOffsetX(static_cast<int>(focusX));
    RenderManager::Get().SetCameraOffsetY(static_cast<int>(focusY));
    RenderManager::Get().SetCenterX(centerX);


}

void Background::Render() {
    Vector2 screenPos = RenderManager::Get().WorldToScreen(position);
    RenderManager::Get().LoopImage(m_ImageKey, true, position.x, screenPos.y);
}

void Background::SetPlayers(Object* p1, Object* p2) {
    m_pPlayer1 = p1;
    m_pPlayer2 = p2;
}
