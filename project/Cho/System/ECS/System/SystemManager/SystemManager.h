#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

#include"ECS/System/PhysicsSystem/PhysicsSystem.h"
#include"ECS/System/RenderSystem/RenderSystem.h"
#include"ECS/System/CameraSystem/CameraSystem.h"
#include"ECS/System/ScriptSystem/ScriptSystem.h"
#include"ECS/System/TransformSystem/TransformSystem.h"
#include"ECS/System/SpriteSystem/SpriteSystem.h"
#include"ECS/System/MaterialSystem/MaterialSystem.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class TextureLoader;

class SystemManager {
public:
    // デストラクタ
    ~SystemManager();

    void Initialize(ResourceViewManager* rvManager,TextureLoader* texLoader,EntityManager* entityManager, ComponentManager* componentManager);

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

    void DebugCameraUpdate(
        CameraComponent& camera
    );

    void DebugDraw(
        EntityManager& entityManager,
        ComponentManager& componentManager,
        D3DCommand* d3dCommand,
        ResourceViewManager* rvManager,
        GraphicsSystem* graphicsSystem,
        TextureLoader* texLoad,
        const CameraComponent& camera
    );

    void PlaySwitch() { run = !run; }
    bool IsPlay() { return run; }

    void LoadScripts();
    void UnLoadScripts();
private:
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
    CameraSystem cameraSystem;
    ScriptSystem scriptSystem;
    TransformSystem transformSystem;
    SpriteSystem spriteSystem;
    MaterialSystem materialSystem;


    EntityManager* entityManager_ = nullptr;
    ComponentManager* componentManager_ = nullptr;

    bool run = false;
};

