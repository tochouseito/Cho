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
        if (meshs.find(entity) == meshs.end()) {
            return nullptr;
        }
        return &meshs[entity];
    }

    // 指定されたエンティティからコンポーネントを削除します。
    void RemoveComponent(Entity entity);
    
private:
    std::unordered_map<Entity, TransformComponent> transforms;  // エンティティのTransformコンポーネントを管理します。
    std::unordered_map<Entity, RenderComponent> renders;        // エンティティのRenderコンポーネントを管理します。
    std::unordered_map<Entity, PhysicsComponent> physics;       // エンティティのPhysicsコンポーネントを管理します。
    std::unordered_map<Entity, MeshComponent> meshs;

    ResourceViewManager* RVManager_ = nullptr;
};