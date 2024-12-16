#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

#include"ECS/System/PhysicsSystem/PhysicsSystem.h"
#include"ECS/System/RenderSystem/RenderSystem.h"
#include"ECS/System/CameraSystem/CameraSystem.h"
#include"ECS/System/ScriptSystem/ScriptSystem.h"
#include"ECS/System/TransformSystem/TransformSystem.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class TextureLoader;
class SystemManager {
public:
    void Start(EntityManager& entityManager, ComponentManager& componentManager);

    void Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime);

    void Draw(
        EntityManager& entityManager,
        ComponentManager& componentManager,
        D3DCommand* d3dCommand,
        ResourceViewManager* rvManager,
        GraphicsSystem* graphicsSystem,
        TextureLoader* texLoad
    );

    // デバッグカメラ
    void DebugDraw(
        EntityManager& entityManager,
        ComponentManager& componentManager,
        D3DCommand* d3dCommand,
        ResourceViewManager* rvManager,
        GraphicsSystem* graphicsSystem,
        TextureLoader* texLoad
    );

    void PlaySwitch() { run = !run; }
    bool IsPlay() { return run; }
private:
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
    CameraSystem cameraSystem;
    ScriptSystem scriptSystem;
    TransformSystem transformSystem;

    bool run = false;
};

