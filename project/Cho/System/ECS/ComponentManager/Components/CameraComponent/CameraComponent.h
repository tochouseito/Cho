#pragma once
#include"ChoMath.h"
#include"SystemState/SystemState.h"
// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection final{
    Matrix4 view;
    Matrix4 projection;
    Matrix4 matWorld;
    Vector3 cameraPosition;
};
struct CameraComponent final{
    Vector3 translation; // 座標
    Vector3 rotation; // 回転

    Matrix4 matWorld; // ワールド行列

    // 垂直方向視野角
    float fovAngleY = 45.0f * std::numbers::pi_v<float> / 180.0f;
    // ビューポートのアスペクト比
    float aspectRatio = (float)16 / 9;
    // 深度限界（手前側）
    float nearZ = 0.1f;
    // 深度限界（奥側）
    float farZ = 1000.0f;

    ConstBufferDataViewProjection* constData = nullptr;
    uint32_t cbvIndex;

    // 初期化
    inline void Initialize() {
        translation.Initialize();
        translation.z = -30.0f;
        rotation.Initialize();
        matWorld = ChoMath::MakeIdentity4x4();

    }
    inline void UpdateMatrix() {
        Quaternion rotate = ChoMath::FromEulerAngles(rotation);
        matWorld = ChoMath::MakeAffineMatrix(Scale(1.0f, 1.0f, 1.0f), rotate, translation);

        TransferMatrix();
    }
    inline void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->view = Matrix4::Inverse(matWorld);
        float width = static_cast<float>(WindowWidth());
        float height = static_cast<float>(WindowHeight());
        constData->projection = ChoMath::MakePerspectiveFovMatrix(
            0.45f, width / height, 0.1f, 100.0f
        );
        constData->cameraPosition = translation;
    }
};