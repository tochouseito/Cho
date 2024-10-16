#pragma once

// Components
#include"ECS/ComponentManager/Components/Components.h"

// EntityManager
#include"ECS/EntityManager/EntityManager.h"

// C++
#include<unordered_map>

class ComponentManager {
public:
    void AddComponent(Entity entity, const TransformComponent& component);

    void AddComponent(Entity entity, const RenderComponent& component);

    void AddComponent(Entity entity, const PhysicsComponent& component);

    inline TransformComponent* GetTransform(Entity entity) {
        return &transforms[entity];
    }

    inline RenderComponent* GetRender(Entity entity) {
        return &renders[entity];
    }

    inline PhysicsComponent* GetPhysics(Entity entity) {
        return &physics[entity];
    }

    void RemoveComponent(Entity entity);
private:
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, RenderComponent> renders;
    std::unordered_map<Entity, PhysicsComponent> physics;
};