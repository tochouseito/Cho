#include "PrecompiledHeader.h"
#include "PhysicsSystem.h"

void PhysicsSystem::Update(EntityManager& entityManager, ComponentManager& componentManager, float deltaTime) {
    for (Entity entity : entityManager.GetActiveEntities()) {
        PhysicsComponent* physics = componentManager.GetPhysics(entity);
        TransformComponent* transform = componentManager.GetTransform(entity);

        // 後で消す
       /* CameraComponent* cameraCompo = componentManager.GetCamera(entity);*/
        if (physics && transform) {
            // 物理計算: 位置の更新
            physics->velocity.x += physics->acceleration.x * deltaTime;
            transform->translation.x += physics->velocity.x * deltaTime;
            // 他の軸も同様に処理
        }
        /*if (cameraCompo) {
            cameraCompo->UpdateMatrix();
        }*/
    }
}
