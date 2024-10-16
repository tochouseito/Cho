#include "GameObject.h"

inline GameObject::GameObject(EntityManager* em, ComponentManager* cm)
    : entityManager(em), componentManager(cm) {
    entity = entityManager->CreateEntity();
}

inline void GameObject::SetParent(GameObject* newParent) {
    parent = newParent;
    newParent->AddChild(this);
}

inline void GameObject::AddChild(GameObject* child) {
    children.push_back(child);
}

inline Entity GameObject::GetEntityID() const {
    return entity;
}

inline void GameObject::AddComponent(const TransformComponent& component) {
    componentManager->AddComponent(entity, component);
}

inline void GameObject::AddComponent(const RenderComponent& component) {
    componentManager->AddComponent(entity, component);
}

inline void GameObject::AddComponent(const PhysicsComponent& component) {
    componentManager->AddComponent(entity, component);
}

inline void GameObject::UpdateChildren(float deltaTime) {
    // 子オブジェクトの更新
    for (auto child : children) {
        child->UpdateChildren(deltaTime);
    }
}
