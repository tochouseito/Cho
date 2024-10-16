#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

#include"ECS/System/PhysicsSystem/PhysicsSystem.h"
#include"ECS/System/RenderSystem/RenderSystem.h"


class SystemManager {
public:
    void Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime);
private:
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
};

