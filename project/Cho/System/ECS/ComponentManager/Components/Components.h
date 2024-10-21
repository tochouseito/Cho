#pragma once

// Utility
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4x4.h"
#include"MyMath/MyMath.h"

// C++
#include<cstdint>
#include<string>
#include<unordered_map>
#include<vector>

// Data
#include"Generator/Mesh/Mesh.h"

// Transform
// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
    Matrix4x4 matWorld; // ローカル → ワールド変換行列
    Matrix4x4 worldInverse;
    Matrix4x4 rootNode;// モデルのRootMatrix
};
struct TransformComponent {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Matrix4x4 matWorld;

    Matrix4x4 rootMatrix;

    // 初期化
    inline void Initialize() {
        position.Initialize();
        rotation.Initialize();
        scale = { 1.0f,1.0f,1.0f };
        matWorld = MyMath::MakeIdentity4x4();
        rootMatrix = MyMath::MakeIdentity4x4();
    }
    inline void UpdateMatrix() {
        matWorld = MyMath::MakeAffineMatrix(scale, rotation, position);
        
        TransferMatrix();
    }
    inline void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->worldInverse = MyMath::Transpose(MyMath::Inverse(matWorld));
        constData->rootNode = rootMatrix;
    }

    ConstBufferDataWorldTransform* constData = nullptr;
    //uint32_t rvIndex;
    uint32_t cbvIndex;
};

struct RenderComponent {
    std::string textureID = "";
    bool visible;
};

struct PhysicsComponent {
    Vector3 velocity;
    Vector3 acceleration;
};

struct MeshComponent {
    std::unordered_map <std::string, MeshData> meshData;
    std::vector<std::string> names;
};

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
    Matrix4x4 view;
    Matrix4x4 projection;
    Matrix4x4 matWorld;
    Vector3 cameraPosition;
};
struct CameraComponent {
    Vector3 position;
    Vector3 rotation;

    Matrix4x4 matWorld;

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
        position.z = -50.0f;
        rotation.Initialize();
        matWorld = MyMath::MakeIdentity4x4();
        
    }
    inline void UpdateMatrix() {
        matWorld = MyMath::MakeAffineMatrix(Vector3(1.0f,1.0f,1.0f), rotation, position);

        TransferMatrix();
    }
    inline void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->view = MyMath::Inverse(matWorld);
        constData->projection = MyMath::MakePerspectiveFovMatrix(
            0.45f, 1280.0f / 720.0f, 0.1f, 100.0f
        );
        constData->cameraPosition = position;
    }
};

struct MaterialComponent {

};