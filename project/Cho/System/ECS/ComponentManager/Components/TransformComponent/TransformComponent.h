#pragma once
// Transform
// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform final{
    Matrix4 matWorld; // ローカル → ワールド変換行列
    Matrix4 worldInverse;
    Matrix4 rootNode;// モデルのRootMatrix
};
struct TransformComponent final {
    Vector3 translation;
    Quaternion rotation;
    Scale scale;

    Matrix4 matWorld;

    Matrix4 rootMatrix;

    // 初期化
    inline void Initialize() {
        translation.Initialize();
        rotation.Initialize();
        scale = { 1.0f,1.0f,1.0f };
        matWorld = ChoMath::MakeIdentity4x4();
        rootMatrix = ChoMath::MakeIdentity4x4();
    }
    inline void UpdateMatrix() {
        matWorld = ChoMath::MakeAffineMatrix(scale, rotation, translation);

        TransferMatrix();
    }
    inline void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->worldInverse = ChoMath::Transpose(ChoMath::Inverse(matWorld));
        constData->rootNode = rootMatrix;
    }

    ConstBufferDataWorldTransform* constData = nullptr;
    //uint32_t rvIndex;
    uint32_t cbvIndex;
};