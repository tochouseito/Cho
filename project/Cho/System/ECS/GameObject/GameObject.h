#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

// C++
#include<string>

class GameObject {

public:
    GameObject(EntityManager* em, ComponentManager* cm);

    void SetParent(GameObject* newParent);

    void AddChild(GameObject* child);

    Entity GetEntityID() const;

    void AddComponent(const TransformComponent& component);

    void AddComponent(const RenderComponent& component);

    void AddComponent(const PhysicsComponent& component);

    inline TransformComponent* GetTransform() {
        return componentManager->GetTransform(entity);
    }

    inline RenderComponent* GetRender() {
        return componentManager->GetRender(entity);
    }

    inline PhysicsComponent* GetPhysics() {
        return componentManager->GetPhysics(entity);
    }

    void UpdateChildren(float deltaTime);

private:
    Entity entity;
    EntityManager* entityManager;
    ComponentManager* componentManager;
    std::vector<GameObject*> children;
    GameObject* parent = nullptr;
};