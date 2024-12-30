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
    Vector3 translation = {0.0f,0.0f,-30.0f}; // 座標
    Vector3 rotation = {0.0f,0.0f,0.0f}; // 回転

    Matrix4 matWorld = ChoMath::MakeIdentity4x4(); // ワールド行列

    // 垂直方向視野角
    float fovAngleY = 45.0f * std::numbers::pi_v<float> / 180.0f;
    // ビューポートのアスペクト比
    float aspectRatio = (float)16 / 9;
    // 深度限界（手前側）
    float nearZ = 0.1f;
    // 深度限界（奥側）
    float farZ = 1000.0f;

    ConstBufferDataViewProjection* constData = nullptr;
    uint32_t cbvIndex=0;
};