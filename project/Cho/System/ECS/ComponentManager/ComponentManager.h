#pragma once

// Components
#include"Components/Components.h"

// EntityManager
#include"ECS/EntityManager/EntityManager.h"

// C++
#include<unordered_map>

class ComponentManager {
public:
    void AddComponent(Entity entity, const TransformComponent& component);

    void AddComponent(Entity entity, const RenderComponent& component);

    void AddComponent(Entity entity, const PhysicsComponent& component);

    TransformComponent* GetTransform(Entity entity);

    RenderComponent* GetRender(Entity entity);

    PhysicsComponent* GetPhysics(Entity entity);

    void RemoveComponent(Entity entity);
private:
    std::unordered_map<Entity, TransformComponent> transforms;
    std::unordered_map<Entity, RenderComponent> renders;
    std::unordered_map<Entity, PhysicsComponent> physics;
};