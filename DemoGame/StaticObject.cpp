//======================================================================
// StaticObject.cpp
//======================================================================


#include "framework.hpp"
#include "StaticObject.hpp"
#include "../GDIEngineLib/RenderManager.hpp"

StaticObject::StaticObject(float x, float y, int width, int height, StaticObjectType type)
	: m_Type(type)
{
	position = Vector2(x, y);
	colliderWidth = width;
	colliderHeight = height;
}


void StaticObject::Update() {
	// 벽/바닥은 움직이지 않음
}

void StaticObject::Render() {

	//RenderManager::Get().DrawColliderBox(GetCollider(), Gdiplus::Color(255, 0, 0, 255)); // 파란색
}
