#pragma once

// Components
#include"ECS/ComponentManager/Components/Components.h"

// EntityManager
#include "ECS/EntityManager/EntityManager.h"

// C++
#include <unordered_map>

// 各エンティティに対してコンポーネントを管理するクラス
class ComponentManager {
public:
    // エンティティにTransformコンポーネントを追加します。
    void AddComponent(Entity entity, const TransformComponent& component);

    // エンティティにRenderコンポーネントを追加します。
    void AddComponent(Entity entity, const RenderComponent& component);

    // エンティティにPhysicsコンポーネントを追加します。
    void AddComponent(Entity entity, const PhysicsComponent& component);

    // 指定されたエンティティのTransformコンポーネントを取得します。
    inline TransformComponent* GetTransform(Entity entity) {
        return &transforms[entity];
    }

    // 指定されたエンティティのRenderコンポーネントを取得します。
    inline RenderComponent* GetRender(Entity entity) {
        return &renders[entity];
    }

    // 指定されたエンティティのPhysicsコンポーネントを取得します。
    inline PhysicsComponent* GetPhysics(Entity entity) {
        return &physics[entity];
    }

    // 指定されたエンティティからコンポーネントを削除します。
    void RemoveComponent(Entity entity);
    
private:
    std::unordered_map<Entity, TransformComponent> transforms;  // エンティティのTransformコンポーネントを管理します。
    std::unordered_map<Entity, RenderComponent> renders;        // エンティティのRenderコンポーネントを管理します。
    std::unordered_map<Entity, PhysicsComponent> physics;       // エンティティのPhysicsコンポーネントを管理します。
};