#pragma once

// ComponentManager
#include "ECS/ComponentManager/ComponentManager.h"

// C++
#include <string>

enum Type {
    Object,
    Camera,
    Light
};

class GameObject {

public:
    GameObject() = default;

    void CreateEntity();

    void CreateCameraEntity();

    void SetManager(EntityManager* em, ComponentManager* cm);

    // 親オブジェクトを設定します。
    void SetParent(GameObject* newParent);

    // 子オブジェクトを追加します。
    void AddChild(GameObject* child);

    // EntityのIDを取得します。
    Entity GetEntityID() const;

    Type GetObjectType() const;

    // TransformComponentを追加します。
    void AddComponent(const TransformComponent& component);

    // RenderComponentを追加します。
    void AddComponent(const RenderComponent& component);

    // PhysicsComponentを追加します。
    void AddComponent(const PhysicsComponent& component);

    void AddComponent(const MeshComponent& component);

    void AddComponent(const CameraComponent& component);

    void AddComponent(const MaterialComponent& component);

    // TransformComponentを取得します。
    inline TransformComponent* GetTransform() {
        return componentManager->GetTransform(entity);
    }

    // RenderComponentを取得します。
    inline RenderComponent* GetRender() {
        return componentManager->GetRender(entity);
    }

    // PhysicsComponentを取得します。
    inline PhysicsComponent* GetPhysics() {
        return componentManager->GetPhysics(entity);
    }

    inline MeshComponent* GetMesh() {
        return componentManager->GetMesh(entity);
    }

    inline MaterialComponent* GetMaterial() {
        return componentManager->GetMaterial(entity);
    }

    inline CameraComponent* GetCamera() {
        return componentManager->GetCamera(entity);
    }

    // 子オブジェクトを更新します。
    void UpdateChildren(float deltaTime);

private:
    Entity entity;
    EntityManager* entityManager;
    ComponentManager* componentManager;
    std::vector<GameObject*> children;
    GameObject* parent = nullptr;
    Type type;
};