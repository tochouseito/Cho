#pragma once
#include"ConstantData/WorldTransform.h"
#include"ChoMath.h"
// Transform

struct TransformComponent final {
    Vector3 translation = { 0.0f, 0.0f, 0.0f };
    Quaternion rotation = { 0.0f, 0.0f, 0.0f,1.0f };
    Scale scale = { 1.0f, 1.0f, 1.0f };

    Matrix4 matWorld = ChoMath::MakeIdentity4x4();
    Matrix4 rootMatrix = ChoMath::MakeIdentity4x4();

    // 度数表示,操作用変数
    Vector3 degrees = { 0.0f,0.0f,0.0f };

    // 差分計算用
    Vector3 prePos = { 0.0f,0.0f,0.0f };
    Vector3 preRot = { 0.0f,0.0f,0.0f };
    Scale preScale = { 1.0f,1.0f,1.0f };

    ConstBufferDataWorldTransform* constData = nullptr;
    
    uint32_t cbvIndex=0;
};