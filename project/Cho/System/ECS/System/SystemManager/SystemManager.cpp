#include "PrecompiledHeader.h"
#include "SystemManager.h"

void SystemManager::Start(EntityManager& entityManager, ComponentManager& componentManager)
{
    scriptSystem.Start(entityManager, componentManager);
}

void SystemManager::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    scriptSystem.Update(entityManager, componentManager);
    physicsSystem.Update(entityManager, componentManager, deltaTime);
    cameraSystem.Update(entityManager, componentManager);
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

void SystemManager::DebugDraw(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad
)
{
    renderSystem.DebugRender(entityManager, componentManager, d3dCommand, rvManager, graphicsSystem, texLoad);
}
