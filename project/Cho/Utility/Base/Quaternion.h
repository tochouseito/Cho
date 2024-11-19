#pragma once
#include <cmath>

/// <summary>
/// クォータニオン構造体
/// </summary>
struct Quaternion final {
    float x, y, z, w;

    // コンストラクタ
    constexpr Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w) {}

    // 初期化
    void Initialize() {
        x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f;
    }

    // 加算（演算子）
    Quaternion operator+(const Quaternion& other) const {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    // 減算（演算子）
    Quaternion operator-(const Quaternion& other) const {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    // スカラー乗算（演算子）
    Quaternion operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar, w * scalar };
    }

    // スカラー除算（演算子）
    Quaternion operator/(float scalar) const {
        if (scalar == 0.0f) {
            return { 0.0f, 0.0f, 0.0f, 1.0f }; // デフォルトの単位クォータニオンを返す
        }
        return { x / scalar, y / scalar, z / scalar, w / scalar };
    }

    // クォータニオンの積 (this * other)
    Quaternion Multiply(const Quaternion& other) const {
        return {
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        };
    }

    // クォータニオン積（演算子）
    Quaternion operator*(const Quaternion& other) const {
        return Multiply(other);
    }

    // 共役クォータニオン
    Quaternion Conjugate() {
        x = -x;
        y = -y;
        z = -z;
        w = w;
    }

    // ノルム（大きさ）の計算
    float Norm() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    // 内積
    float Dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // 正規化
    Quaternion Normalize() {
        float norm = Norm();
        if (norm == 0.0f) {
            x = 0.0f;y = 0.0f;z = 0.0f;w = 1.0f;
        } else {
            x /= norm;y /= norm;z /= norm;w /= norm;
        }
        return { x,y,z,w };
    }

    // 逆クォータニオン
    Quaternion Inverse() {
        Quaternion conjugate = Conjugate();
        float norm = Norm();
        float normSq = norm * norm;
        if (normSq == 0.0f) {
            x = 0.0f;y = 0.0f;z = 0.0f;w = 1.0f;
        } else {
            x = conjugate.x / normSq;y = conjugate.y / normSq;
            z = conjugate.z / normSq; w = conjugate.w / normSq;
        }
        return{ x,y,z,w };
    }
public:// 静的メンバ

    // 線形補間（Lerp）
    static Quaternion Lerp(const Quaternion& start, const Quaternion& end, float t) {
        Quaternion result = (start * (1.0f - t) + end * t);
        result.Normalize();
        return result;
    }

    // 球面線形補間（Slerp）
    static Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t) {
        float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
        const float threshold = 0.9995f;

        // 線形補間で近似
        if (dot > threshold) {
            return Lerp(start, end, t);
        }

        dot = std::fmax(std::fmin(dot, 1.0f), -1.0f);
        float theta_0 = std::acos(dot);
        float theta = theta_0 * t;

        Quaternion relative = (end - start * dot);
        relative.Normalize();
        return start * std::cos(theta) + relative * std::sin(theta);
    }

    // 単位クォータニオン
    static Quaternion Identity() {
        return { 0.0f, 0.0f, 0.0f, 1.0f };
    }
};