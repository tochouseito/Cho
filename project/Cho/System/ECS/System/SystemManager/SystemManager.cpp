#include "PrecompiledHeader.h"
#include "SystemManager.h"

void SystemManager::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    physicsSystem.Update(entityManager, componentManager, deltaTime);
}

void SystemManager::Draw(EntityManager& entityManager, ComponentManager& componentManager)
{
    renderSystem.Render(entityManager, componentManager);
}
