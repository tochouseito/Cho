#include "PrecompiledHeader.h"
#include "GameObject.h"

void GameObject::CreateEntity()
{
    entity = entityManager->CreateEntity();
    type = Type::Object;
}

void GameObject::CreateCameraEntity()
{
    entity = entityManager->CreateCameraEntity();
    type = Type::Camera;
}

void GameObject::SetManager(EntityManager* em, ComponentManager* cm)
{
    entityManager = em;
    componentManager = cm;
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

Type GameObject::GetObjectType() const
{
    return type;
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

void GameObject::AddComponent(const MeshComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const CameraComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::UpdateChildren(float deltaTime) {
    // 子オブジェクトの更新
    for (auto child : children) {
        child->UpdateChildren(deltaTime);
    }
}
