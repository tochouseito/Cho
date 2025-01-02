#include "PrecompiledHeader.h"
#include "ComponentManager.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// 後で消す
#include<numbers>

void ComponentManager::SetRVManager(ResourceViewManager* RVManager)
{
    rvManager_ = RVManager;
}

// EntityにTransformComponentを追加します。
// 指定されたEntityにTransformComponentをTransformsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
    transforms[entity].cbvIndex = rvManager_->CreateCBV(sizeof(ConstBufferDataWorldTransform));
    rvManager_->GetCBVResource(
        transforms[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&transforms[entity].constData)
        );
    // 初期化
    transforms[entity].constData->matWorld = transforms[entity].matWorld;
    transforms[entity].constData->worldInverse = Transpose(Matrix4::Inverse(transforms[entity].matWorld));
    transforms[entity].constData->rootNode = transforms[entity].rootMatrix;
}

// EntityにRenderComponentを追加します。
// 指定されたEntityにRenderComponentをRendersマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
	renders[entity].visible = true;
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
	cameras[entity].cbvIndex = rvManager_->CreateCBV(sizeof(ConstBufferDataViewProjection));
	rvManager_->GetCBVResource(
		cameras[entity].cbvIndex)->Map(
			0, nullptr, reinterpret_cast<void**>(&cameras[entity].constData)
		);
    // 初期化
    cameras[entity].constData->matWorld = cameras[entity].matWorld;
    cameras[entity].constData->view = Matrix4::Inverse(cameras[entity].matWorld);
    cameras[entity].constData->projection = MakePerspectiveFovMatrix(
        cameras[entity].fovAngleY, cameras[entity].aspectRatio, cameras[entity].nearZ, cameras[entity].farZ
    );
    cameras[entity].constData->cameraPosition = cameras[entity].translation;
}

void ComponentManager::AddComponent(Entity entity, const MaterialComponent& component)
{
    materials[entity] = component;
    // デフォルト画像テクスチャマテリアル
    std::string texName= "uvChecker.png";
    materials[entity].textureID = texName;
    materials[entity].preTexID = texName;
    materials[entity].cbvIndex = rvManager_->CreateCBV(sizeof(ConstBufferDataMaterial));
    rvManager_->GetCBVResource(
        materials[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&materials[entity].constData)
        );
    // 初期化
    materials[entity].constData->matUV = ChoMath::MakeIdentity4x4();
    materials[entity].constData->color = { 1.0f,1.0f,1.0f,1.0f };
    materials[entity].constData->enableLighting = false;
    materials[entity].constData->shininess = 50.0f;
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
    case ObjectType::Sprite:
        spriteScripts[entity] = component;
        break;
    default:
        break;
    }
}

void ComponentManager::AddComponent(Entity entity, const SpriteComponent& component)
{
    sprites[entity] = component;
    sprites[entity].cbvIndex = rvManager_->CreateCBV(sizeof(ConstBufferDataSprite));
    rvManager_->GetCBVResource(
        sprites[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&sprites[entity].constData)
        );
    // 初期化
    sprites[entity].constData->matWorld = sprites[entity].matWorld;
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

void ComponentManager::InitCameraComponent(CameraComponent* camera)
{
    camera->cbvIndex=rvManager_->CreateCBV(sizeof(ConstBufferDataViewProjection));
    rvManager_->GetCBVResource(
        camera->cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&camera->constData)
        );
}

