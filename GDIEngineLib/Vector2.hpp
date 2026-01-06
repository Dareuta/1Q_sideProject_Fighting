//======================================================================
// Vector2.hpp
//======================================================================


#pragma once
#include <cmath>

constexpr double MY_EPSILON = 1e-5;

// 2D 벡터 유틸리티 클래스
struct Vector2 {
    double x;
    double y;

    // 기본 생성자 (0, 0)
    Vector2() : x(0.0), y(0.0) {}

    // double 기반 생성자
    Vector2(double x, double y) : x(x), y(y) {}

    // float → double 변환 생성자
    Vector2(float x, float y) : x(static_cast<double>(x)), y(static_cast<double>(y)) {}

    // int → double 변환 생성자
    Vector2(int x, int y) : x(static_cast<double>(x)), y(static_cast<double>(y)) {}

    // 덧셈, 뺄셈, 스칼라 곱/나눗셈
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(double scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(double scalar) const {
        return (std::abs(scalar) <= MY_EPSILON) ? Vector2(0.0, 0.0) : Vector2(x / scalar, y / scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    Vector2& operator*=(double scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }

    Vector2& operator/=(double scalar) {
        if (std::abs(scalar) > MY_EPSILON) {
            x /= scalar; y /= scalar;
        }
        else {
            x = y = 0.0;
        }
        return *this;
    }

    // 비교 연산자 (부동소수 오차 고려)
    bool operator==(const Vector2& other) const {
        return std::abs(x - other.x) <= MY_EPSILON && std::abs(y - other.y) <= MY_EPSILON;
    }

    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    // 벡터 길이 (제곱 및 루트)
    double LengthSQ() const {
        return x * x + y * y;
    }

    double Length() const {
        return std::sqrt(LengthSQ());
    }

    // 단위 벡터
    Vector2 Normalized() const {
        double len = Length();
        return (len <= MY_EPSILON) ? Vector2(0.0, 0.0) : *this / len;
    }

    // 0 벡터 판정
    bool IsZero() const {
        return std::abs(x) <= MY_EPSILON && std::abs(y) <= MY_EPSILON;
    }

    // 내적
    double Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    static double Dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }

    // 거리
    static double Distance(const Vector2& a, const Vector2& b) {
        return (a - b).Length();
    }

    // 선형 보간 (Lerp)
    static Vector2 Lerp(const Vector2& a, const Vector2& b, double t) {
        return a + (b - a) * t;
    }
};
