#pragma once

// Utility
#include"Vector3.h"

// C++
#include<cstdint>
#include<string>

struct TransformComponent {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
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

};