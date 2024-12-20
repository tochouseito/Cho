#pragma once
#include"ChoMath.h"
// Transform
// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform final{
    Matrix4 matWorld; // ローカル → ワールド変換行列
    Matrix4 worldInverse;
    Matrix4 rootNode;// モデルのRootMatrix
};
struct TransformComponent final {
    Vector3 translation = { 0.0f, 0.0f, 0.0f };
    Quaternion rotation = { 0.0f, 0.0f, 0.0f,1.0f };
    Scale scale = { 1.0f, 1.0f, 1.0f };

    Matrix4 matWorld = ChoMath::MakeIdentity4x4();
    Matrix4 rootMatrix = ChoMath::MakeIdentity4x4();

    // 操作用のクオータニオンの3要素
    Vector3 rot= { 0.0f, 0.0f, 0.0f };
    // 差分計算用
    Quaternion diffQ = { 0.0f, 0.0f, 0.0f,1.0f };
    Vector3 diffRot= { 0.0f, 0.0f, 0.0f };

    ConstBufferDataWorldTransform* constData = nullptr;
    //uint32_t rvIndex;
    uint32_t cbvIndex;
};