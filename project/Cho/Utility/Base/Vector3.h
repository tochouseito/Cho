#pragma once
#include <cmath>

/// <summary>
/// 3次元ベクトル構造体
/// </summary>
struct Vector3 final {
    float x, y, z;

    // コンストラクタ
    constexpr Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z) {}

    // 初期化
    void Initialize() {
        x = 0.0f; y = 0.0f; z = 0.0f;
    }

    // ベクトル加算
    Vector3 operator+(const Vector3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    // ベクトル減算
    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    // スカラー乗算
    Vector3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    // ベクトルの長さ
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // 正規化
    void Normalize() {
        float len = Length();
        if (len == 0.0f) {
            Initialize();
        }
        x = x / len;
        y = y / len;
        z = z / len;
    }

    // 内積
    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // 外積
    Vector3 Cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    // 距離計算
    float Distance(const Vector3& other) const {
        return std::sqrt(
            (x - other.x) * (x - other.x) +
            (y - other.y) * (y - other.y) +
            (z - other.z) * (z - other.z)
        );
    }

    // ベクトルの他のベクトルへの投影
    Vector3 ProjectOnto(const Vector3& other) const {
        float dot = Dot(other);
        float otherLengthSq = other.Length() * other.Length();
        if (otherLengthSq == 0.0f) return { 0.0f, 0.0f, 0.0f }; // 長さがゼロの場合
        return other * (dot / otherLengthSq);
    }
};