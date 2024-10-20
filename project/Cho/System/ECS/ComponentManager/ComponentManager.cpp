#include "ComponentManager.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void ComponentManager::SetRVManager(ResourceViewManager* RVManager)
{
    RVManager_ = RVManager;
}

// EntityにTransformComponentを追加します。
// 指定されたentityにTransformComponentをtransformsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
    transforms[entity].cbvIndex = RVManager_->CreateCBV(sizeof(ConstBufferDataWorldTransform));
    RVManager_->GetCBVResource(
        transforms[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&transforms[entity].constData)
        );
}

// EntityにRenderComponentを追加します。
// 指定されたentityにRenderComponentをrendersマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
}

// EntityにPhysicsComponentを追加します。
// 指定されたentityにPhysicsComponentをphysicsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const PhysicsComponent& component) {
    physics[entity] = component;
}

void ComponentManager::AddComponent(Entity entity, const MeshComponent& component)
{
    meshs[entity] = component;
}

// Entityに関連するすべてのコンポーネントを削除します。
// 指定されたentityに対して、TransformComponent、RenderComponent、PhysicsComponentをそれぞれのマップから削除します。
void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
    meshs.erase(entity);
}
