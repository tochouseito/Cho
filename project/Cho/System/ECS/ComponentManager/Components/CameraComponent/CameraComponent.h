#pragma once
// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection final{
    Matrix4 view;
    Matrix4 projection;
    Matrix4 matWorld;
    Vector3 cameraPosition;
};
struct CameraComponent final{
    Vector3 position; // 座標
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
        position.Initialize();
        position.z = -30.0f;
        rotation.Initialize();
        matWorld = MyMath::MakeIdentity4x4();

    }
    inline void UpdateMatrix() {
        matWorld = MyMath::MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), rotation, position);

        TransferMatrix();
    }
    inline void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->view = MyMath::Inverse(matWorld);
        float width = static_cast<float>(WindowWidth());
        float height = static_cast<float>(WindowHeight());
        constData->projection = MyMath::MakePerspectiveFovMatrix(
            0.45f, width / height, 0.1f, 100.0f
        );
        constData->cameraPosition = position;
    }
};