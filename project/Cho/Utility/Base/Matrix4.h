#pragma once
#include <cmath>
#include <cstring>

/// <summary>
/// 4x4行列構造体
/// </summary>
struct Matrix4 final {
    float m[4][4];

    // コンストラクタ（ゼロ初期化）
    Matrix4() {
        std::memset(m, 0, sizeof(m));
    }

    // 単位行列として初期化
    void InitializeIdentity() {
        std::memset(m, 0, sizeof(m));
        for (int i = 0; i < 4; ++i) {
            m[i][i] = 1.0f;
        }
    }

    // 行列加算
    Matrix4 operator+(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m[i][j] + other.m[i][j];
            }
        }
        return result;
    }

    // 行列減算
    Matrix4 operator-(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m[i][j] - other.m[i][j];
            }
        }
        return result;
    }

    // スカラー乗算
    Matrix4 operator*(float scalar) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m[i][j] * scalar;
            }
        }
        return result;
    }

    // 行列積
    Matrix4 Multiply(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // 行列積（演算子）
    Matrix4 operator*(const Matrix4& other) const {
        return Multiply(other);
    }

    // 転置行列
    Matrix4 Transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }

    // 逆行列（簡易版：完全な実装は必要に応じて調整）
    Matrix4 Inverse() const {
        // Determinantや行列の計算コードを実装（ここでは省略）
        // 完全な逆行列のコードを準備してください。
        Matrix4 result;
        // デモ用に単位行列を返す
        result.InitializeIdentity();
        return result;
    }

public:// 静的メンバ

    // 単位行列
    static Matrix4 Identity() {
        Matrix4 mat;
        mat.InitializeIdentity();
        return mat;
    }
};