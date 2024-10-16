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

    TransformComponent* GetTransform();

    RenderComponent* GetRender();

    PhysicsComponent* GetPhysics();

    void UpdateChildren(float deltaTime);

private:
    Entity entity;
    EntityManager* entityManager;
    ComponentManager* componentManager;
    std::vector<GameObject*> children;
    GameObject* parent = nullptr;
};