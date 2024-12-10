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
            other.w * x + other.x * w + other.y * z - other.z * y,
            other.w * y - other.x * z + other.y * w + other.z * x,
            other.w * z + other.x * y - other.y * x + other.z * w,
            other.w * w - other.x * x - other.y * y - other.z * z
        };
    }

    // クォータニオン積（演算子）
    Quaternion operator*(const Quaternion& other) const {
        return Multiply(other);
    }

    // 共役クォータニオン
    void Conjugate() {
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
    void Normalize() {
        float norm = Norm();
        if (norm == 0.0f) {
            x = 0.0f;y = 0.0f;z = 0.0f;w = 1.0f;
        } else {
            x /= norm;y /= norm;z /= norm;w /= norm;
        }
    }

    // 逆クォータニオン
    void Inverse() {
        Quaternion q = { x,y,z,w };
        Quaternion conjugate = Quaternion::Conjugate(q);
        float norm = Norm();
        float normSq = norm * norm;
        if (normSq == 0.0f) {
            x = 0.0f;y = 0.0f;z = 0.0f;w = 1.0f;
        } else {
            x = conjugate.x / normSq;y = conjugate.y / normSq;
            z = conjugate.z / normSq; w = conjugate.w / normSq;
        }
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

    // 共役クォータニオン
    static Quaternion Conjugate(const Quaternion& q) {
        Quaternion result;
        result.x = -q.x;
        result.y = -q.y;
        result.z = -q.z;
        result.w = q.w;
        return result;
    }

    // 正規化
    static Quaternion Normalize(const Quaternion& q) {
        Quaternion result;
        float norm = q.Norm();
        if (norm == 0.0f) {
            result.x = 0.0f; result.y = 0.0f; result.z = 0.0f; result.w = 1.0f;
        } else {
            result.x /= norm; result.y /= norm; result.z /= norm; result.w /= norm;
        }
        return result;
    }

    // 逆クォータニオン
    static Quaternion Inverse(const Quaternion& q) {
        Quaternion result;
        Quaternion conjugate = Quaternion::Conjugate(q);
        float norm = q.Norm();
        float normSq = norm * norm;
        if (normSq == 0.0f) {
            result.x = 0.0f; result.y = 0.0f; result.z = 0.0f; result.w = 1.0f;
        } else {
            result.x = conjugate.x / normSq; result.y = conjugate.y / normSq;
            result.z = conjugate.z / normSq; result.w = conjugate.w / normSq;
        }
        return result;
    }
};