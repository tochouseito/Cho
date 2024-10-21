#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

#include"ECS/System/PhysicsSystem/PhysicsSystem.h"
#include"ECS/System/RenderSystem/RenderSystem.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class TextureLoader;
class SystemManager {
public:
    void Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime);

    void Draw(
        EntityManager& entityManager,
        ComponentManager& componentManager,
        D3DCommand* d3dCommand,
        ResourceViewManager* rvManager,
        GraphicsSystem* graphicsSystem,
        TextureLoader* texLoad
    );
private:
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
};

