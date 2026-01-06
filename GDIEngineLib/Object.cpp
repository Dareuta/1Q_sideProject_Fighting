//======================================================================
//Object.cpp
//======================================================================
#include "pch.hpp"
#include "Object.hpp"
#include "RenderManager.hpp"


RECT Object::GetCollider() {
    float screenX = RenderManager::Get().GetCurrentX(stateKey, position.x);
    float screenY = RenderManager::Get().GetCurrentY(stateKey, position.y);

    return RECT{
        static_cast<int>(screenX),
        static_cast<int>(screenY),
        static_cast<int>(screenX + colliderWidth),
        static_cast<int>(screenY + colliderHeight)
    };
}


bool Object::IsAABBCollision(Object* a, Object* b) {
    RECT r1 = a->GetCollider();
    RECT r2 = b->GetCollider();

    if (r1.bottom < r2.top) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.left > r2.right) return false;
    if (r1.right < r2.left) return false;

    return true;
}

bool Object::IsAABBCollisionRect(const RECT& a, const RECT& b) {
    return !(a.right < b.left || a.left > b.right || a.bottom < b.top || a.top > b.bottom);
}
