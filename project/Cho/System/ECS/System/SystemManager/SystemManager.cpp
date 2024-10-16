#include "PrecompiledHeader.h"
#include "SystemManager.h"

inline void SystemManager::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    physicsSystem.Update(entityManager, componentManager, deltaTime);
    renderSystem.Render(entityManager, componentManager);
}
