#include "ComponentManager.h"

// EntityにTransformComponentを追加します。
// 指定されたentityにTransformComponentをtransformsマップに割り当てます。
inline void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
}

// EntityにRenderComponentを追加します。
// 指定されたentityにRenderComponentをrendersマップに割り当てます。
inline void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
}

// EntityにPhysicsComponentを追加します。
// 指定されたentityにPhysicsComponentをphysicsマップに割り当てます。
inline void ComponentManager::AddComponent(Entity entity, const PhysicsComponent& component) {
    physics[entity] = component;
}

// Entityに関連するすべてのコンポーネントを削除します。
// 指定されたentityに対して、TransformComponent、RenderComponent、PhysicsComponentをそれぞれのマップから削除します。
inline void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
}
