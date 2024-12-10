#include "PrecompiledHeader.h"
#include "ComponentManager.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// 後で消す
#include<numbers>

void ComponentManager::SetRVManager(ResourceViewManager* RVManager)
{
    RVManager_ = RVManager;
}

// EntityにTransformComponentを追加します。
// 指定されたEntityにTransformComponentをTransformsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
    transforms[entity].cbvIndex = RVManager_->CreateCBV(sizeof(ConstBufferDataWorldTransform));
    RVManager_->GetCBVResource(
        transforms[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&transforms[entity].constData)
        );
}

// EntityにRenderComponentを追加します。
// 指定されたEntityにRenderComponentをRendersマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
	renders[entity].visible = true;
	//renders[entity].textureID = "monsterBall.png";
}

// EntityにPhysicsComponentを追加します。
// 指定されたEntityにPhysicsComponentをPhysicsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const PhysicsComponent& component) {
    physics[entity] = component;
}

void ComponentManager::AddComponent(Entity entity, const MeshComponent& component)
{
    meshes[entity] = component;
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

void ComponentManager::AddComponent(Entity entity, const MaterialComponent& component)
{
    materials[entity] = component;
    // デフォルト画像テクスチャマテリアル
    materials[entity].textureID = "uvChecker.png";
}

void ComponentManager::AddComponent(Entity entity, const ScriptComponent& component)
{
    ObjectType type = static_cast<ObjectType>(component.type);
    switch (type)
    {
    case ObjectType::Object:
        objectScripts[entity] = component;
        break;
    case ObjectType::Camera:
        cameraScripts[entity] = component;
        break;
    case ObjectType::Light:
        lightScripts[entity] = component;
        break;
    default:
        break;
    }
}

// Entityに関連するすべてのコンポーネントを削除します。
// 指定されたEntityに対して、TransformComponent、RenderComponent、PhysicsComponentをそれぞれのマップから削除します。
void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
    meshes.erase(entity);
	cameras.erase(entity);
}
