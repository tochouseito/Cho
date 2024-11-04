#include "ComponentManager.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// 後で消す
#include<numbers>

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
	renders[entity].visible = true;
	renders[entity].textureID = "Cho/Resources/Texture/monsterBall.png";
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

void ComponentManager::AddComponent(Entity entity, const CameraComponent& component)
{
	cameras[entity] = component;
	cameras[entity].cbvIndex = RVManager_->CreateCBV(sizeof(ConstBufferDataViewProjection));
	RVManager_->GetCBVResource(
		cameras[entity].cbvIndex)->Map(
			0, nullptr, reinterpret_cast<void**>(&cameras[entity].constData)
		);
}

// Entityに関連するすべてのコンポーネントを削除します。
// 指定されたentityに対して、TransformComponent、RenderComponent、PhysicsComponentをそれぞれのマップから削除します。
void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
    meshs.erase(entity);
	cameras.erase(entity);
}
