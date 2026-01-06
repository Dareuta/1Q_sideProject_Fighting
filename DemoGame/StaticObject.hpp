//======================================================================
// StaticObject.hpp
//======================================================================


#pragma once
#include "../GDIEngineLib/Object.hpp"

enum class StaticObjectType {
    Ground,
    Wall
};

class StaticObject : public Object {
private:
    StaticObjectType m_Type;

public:
    StaticObject(float x, float y, int width, int height, StaticObjectType type);
    ~StaticObject() override = default;

    void Update() override;
    void Render() override;

    StaticObjectType GetType() const {
        return m_Type;
    }
};
