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

inline TransformComponent* ComponentManager::GetTransform(Entity entity) {
    return &transforms[entity];
}

inline RenderComponent* ComponentManager::GetRender(Entity entity) {
    return &renders[entity];
}

inline PhysicsComponent* ComponentManager::GetPhysics(Entity entity) {
    return &physics[entity];
}

inline void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
}
