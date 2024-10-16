#include "EntityManager.h"

inline Entity EntityManager::CreateEntity() {
    Entity entity = nextEntityID++;
    activeEntities.push_back(entity);
    return entity;
}

inline void EntityManager::DestroyEntity(Entity entity) {
    activeEntities.erase(std::remove(activeEntities.begin(), activeEntities.end(), entity), activeEntities.end());
}
