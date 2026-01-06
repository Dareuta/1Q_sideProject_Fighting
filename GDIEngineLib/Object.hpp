//======================================================================
//Object.hpp
//======================================================================
#pragma once

class Object {
public:
	Object() = default;
	virtual ~Object() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	// 위치 관련
	float GetMoveX() const { return position.x; }
	float GetMoveY() const { return position.y; }
	void SetPos(float x, float y) { position = Vector2(x, y); }
	void SetPos(const Vector2& vec) { position = vec; }
	const Vector2& GetPosition() const { return position; }
	void SetPosition(const Vector2& pos) { position = pos; }

	// 계층
	int GetLayer() const { return m_Layer; }
	void SetLayer(int value) { m_Layer = value; }

	std::wstring GetStateKey() const { return stateKey; }

	// 충돌 영역
	RECT GetCollider();

	// 충돌 판정
	static bool IsAABBCollision(Object* a, Object* b);
	bool IsAABBCollisionRect(const RECT& a, const RECT& b);

protected:
	Vector2 position;          // ← moveX, moveY 대신
	Vector2 size;              // ← colliderWidth, colliderHeight 대신 추천 (선택 사항)

	int colliderWidth = 0;
	int colliderHeight = 0;

	int frameIndex = 0;
	int maxFrame = 0;
	int frameCount = 0;

	int m_Layer = 0;
	std::wstring stateKey = L"";
};
