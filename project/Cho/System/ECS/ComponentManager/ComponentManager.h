#pragma once

// Components
#include"ECS/ComponentManager/Components/Components.h"

// EntityManager
#include "ECS/EntityManager/EntityManager.h"

// C++
#include <unordered_map>

class ResourceViewManager;
// 各エンティティに対してコンポーネントを管理するクラス
class ComponentManager {
public:
    void SetRVManager(ResourceViewManager* RVManager);

    // エンティティにTransformコンポーネントを追加します。
    void AddComponent(Entity entity, const TransformComponent& component);

    // エンティティにRenderコンポーネントを追加します。
    void AddComponent(Entity entity, const RenderComponent& component);

    // エンティティにPhysicsコンポーネントを追加します。
    void AddComponent(Entity entity, const PhysicsComponent& component);

    void AddComponent(Entity entity, const MeshComponent& component);

    void AddComponent(Entity entity, const CameraComponent& component);

    void AddComponent(Entity entity, const MaterialComponent& component);

    // 指定されたエンティティのTransformコンポーネントを取得します。
    inline TransformComponent* GetTransform(Entity entity) {
        if (transforms.find(entity) == transforms.end()) {
            return nullptr;
        }
        return &transforms[entity];
    }

    // 指定されたエンティティのRenderコンポーネントを取得します。
    inline RenderComponent* GetRender(Entity entity) {
        if (renders.find(entity) == renders.end()) {
            return nullptr;
        }
        return &renders[entity];
    }

    // 指定されたエンティティのPhysicsコンポーネントを取得します。
    inline PhysicsComponent* GetPhysics(Entity entity) {
        if (physics.find(entity) == physics.end()) {
            return nullptr;
        }
        return &physics[entity];
    }

    inline MeshComponent* GetMesh(Entity entity) {
        if (meshes.find(entity) == meshes.end()) {
            return nullptr;
        }
        return &meshes[entity];
    }

    inline MaterialComponent* GetMaterial(Entity entity) {
        if (materials.find(entity) == materials.end()) {
            return nullptr;
        }
        return &materials[entity];
    }

    inline CameraComponent* GetCamera(Entity entity) {
        if (cameras.find(entity) == cameras.end()) {
            return nullptr;
        }
        return &cameras[entity];
    }
    // デバッグ用
    inline CameraComponent* GetDebugCamera() {
        if (cameras.find(0) == cameras.end()) {
            return nullptr;
        }
        return &cameras[0];
    }

    // 指定されたエンティティからコンポーネントを削除します。
    void RemoveComponent(Entity entity);
    
private:
    std::unordered_map<Entity, TransformComponent> transforms;  // エンティティのTransformコンポーネントを管理します。
    std::unordered_map<Entity, RenderComponent> renders;        // エンティティのRenderコンポーネントを管理します。
    std::unordered_map<Entity, PhysicsComponent> physics;       // エンティティのPhysicsコンポーネントを管理します。
    std::unordered_map<Entity, MeshComponent> meshes;
    std::unordered_map<Entity, CameraComponent> cameras;
    std::unordered_map<Entity, MaterialComponent> materials;

    ResourceViewManager* RVManager_ = nullptr;
};