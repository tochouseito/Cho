#include "PrecompiledHeader.h"
#include "SystemManager.h"

void SystemManager::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    physicsSystem.Update(entityManager, componentManager, deltaTime);
}

void SystemManager::Draw(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager
)
{
    renderSystem.Render(entityManager, componentManager,d3dCommand,rvManager);
}
