#include "PrecompiledHeader.h"
#include "SystemManager.h"

SystemManager::~SystemManager()
{
    scriptSystem.Cleanup(*entityManager_,*componentManager_);
}

void SystemManager::Initialize(ResourceViewManager* rvManager,TextureLoader* texLoader, EntityManager* entityManager, ComponentManager* componentManager)
{
    spriteSystem.SetRVManagerPtr(rvManager);
    spriteSystem.SetTexLoadPtr(texLoader);
    entityManager_ = entityManager;
    componentManager_ = componentManager;
}

void SystemManager::Start(EntityManager& entityManager, ComponentManager& componentManager)
{
    scriptSystem.Start(entityManager, componentManager);
    transformSystem.Initialize(entityManager, componentManager);
}

void SystemManager::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    if (run) {
        scriptSystem.Update(entityManager, componentManager);
        materialSystem.Update(entityManager, componentManager);
        spriteSystem.Update(entityManager, componentManager);
        transformSystem.Update(entityManager, componentManager);
        physicsSystem.Update(entityManager, componentManager, deltaTime);
        cameraSystem.Update(entityManager, componentManager);
    } else {
        spriteSystem.Update(entityManager, componentManager);
        materialSystem.Update(entityManager, componentManager);
        transformSystem.UpdateEditor(entityManager, componentManager);
        cameraSystem.UpdateEditor(entityManager, componentManager);
    }
}

void SystemManager::Draw(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad
)
{
    renderSystem.Render(entityManager, componentManager,d3dCommand,rvManager,graphicsSystem,texLoad);
}
void SystemManager::DebugCameraUpdate(CameraComponent& camera)
{
    cameraSystem.UpdateDebugCamera(camera);
}
void SystemManager::DebugDraw(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad,
    const CameraComponent& camera
)
{
    renderSystem.DebugRender(entityManager, componentManager, d3dCommand, rvManager, graphicsSystem, texLoad,camera);
}

void SystemManager::LoadScripts()
{
    scriptSystem.LoadScripts(*entityManager_, *componentManager_);
}

void SystemManager::UnLoadScripts()
{
    scriptSystem.UnLoadDLL(*entityManager_, *componentManager_);
}
