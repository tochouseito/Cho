#include "GameObject.h"

GameObject::GameObject(EntityManager* em, ComponentManager* cm)
    : entityManager(em), componentManager(cm) {
    entity = entityManager->CreateEntity();
}

void GameObject::SetParent(GameObject* newParent) {
    parent = newParent;
    newParent->AddChild(this);
}

void GameObject::AddChild(GameObject* child) {
    children.push_back(child);
}

Entity GameObject::GetEntityID() const {
    return entity;
}

void GameObject::AddComponent(const TransformComponent& component) {
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const RenderComponent& component) {
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const PhysicsComponent& component) {
    componentManager->AddComponent(entity, component);
}

void GameObject::UpdateChildren(float deltaTime) {
    // 子オブジェクトの更新
    for (auto child : children) {
        child->UpdateChildren(deltaTime);
    }
}
