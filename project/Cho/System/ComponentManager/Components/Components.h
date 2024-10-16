#pragma once

// Utility
#include"Vector3.h"

// C++
#include<cstdint>

struct TransformComponent {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
};

struct RenderComponent {
    uint32_t textureID;
    uint32_t viewID;
    bool visible;
};

struct PhysicsComponent {
    Vector3 velocity;
    Vector3 acceleration;
};