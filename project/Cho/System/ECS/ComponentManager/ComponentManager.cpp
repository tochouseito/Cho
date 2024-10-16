#include "ComponentManager.h"

inline void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
}

inline void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
}

inline void ComponentManager::AddComponent(Entity entity, const PhysicsComponent& component) {
    physics[entity] = component;
}

inline void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
}
