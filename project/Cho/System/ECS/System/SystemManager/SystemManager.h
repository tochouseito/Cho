#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

#include"ECS/System/PhysicsSystem/PhysicsSystem.h"
#include"ECS/System/RenderSystem/RenderSystem.h"


class SystemManager {
public:
    void Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime);

    void Draw(EntityManager& entityManager, ComponentManager& componentManager);
private:
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
};

