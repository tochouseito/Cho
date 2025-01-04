#include "PrecompiledHeader.h"
#include "GameObject.h"

void GameObject::CreateEntity()
{
    entity = entityManager->CreateEntity();
    type = ObjectType::Object;
}

void GameObject::CreateCameraEntity()
{
    entity = entityManager->CreateCameraEntity();
    type = ObjectType::Camera;
}

void GameObject::CreateSpriteEntity()
{
    entity = entityManager->CreateSpriteEntity();
    type = ObjectType::Sprite;
}

void GameObject::CreateParticleEntity()
{
    entity = entityManager->CreateParticleEntity();
    type = ObjectType::Particle;
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

ObjectType GameObject::GetObjectType() const
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

void GameObject::AddComponent(const MaterialComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const ScriptComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const SpriteComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const ParticleComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::AddComponent(const EmitterComponent& component)
{
    componentManager->AddComponent(entity, component);
}

void GameObject::UpdateChildren(float deltaTime) {
    // 子オブジェクトの更新
    for (auto child : children) {
        child->UpdateChildren(deltaTime);
    }
}
