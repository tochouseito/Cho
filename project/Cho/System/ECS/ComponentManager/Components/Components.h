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
    void Initialize() {
        position.Initialize();
        rotation.Initialize();
        scale = { 1.0f,1.0f,1.0f };
        matWorld = MyMath::MakeIdentity4x4();
        rootMatrix = MyMath::MakeIdentity4x4();
    }
    void UpdateMatrix() {
        matWorld = MyMath::MakeAffineMatrix(scale, rotation, position);
        
        TransferMatrix();
    }
    void TransferMatrix() {
        constData->matWorld = matWorld;
        constData->worldInverse = MyMath::Transpose(MyMath::Inverse(matWorld));
        constData->rootNode = rootMatrix;
    }

    ConstBufferDataWorldTransform* constData = nullptr;
    //uint32_t rvIndex;
    uint32_t cbvIndex;
};

struct RenderComponent {
    std::string textureID;
    uint32_t viewID;
    bool visible;
};

struct PhysicsComponent {
    Vector3 velocity;
    Vector3 acceleration;
};

struct MeshComponent {
    std::unordered_map <std::string, MeshData> meshData;
};