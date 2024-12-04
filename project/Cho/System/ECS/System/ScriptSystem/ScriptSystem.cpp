#include "PrecompiledHeader.h"
#include "ScriptSystem.h"

void ScriptSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
       // PhysicsComponent* physics = componentManager.GetPhysics(entity);
       // TransformComponent* transform = componentManager.GetTransform(entity);
        ScriptComponent* script = componentManager.GetScript(entity);
        script;
       // // 後で消す
       ///* CameraComponent* cameraCompo = componentManager.GetCamera(entity);*/
       // if (physics && transform) {
       //     // 物理計算: 位置の更新
       //     // 他の軸も同様に処理
       // } else if (!physics && transform)
       // {
       //     transform->UpdateMatrix();
       // }
    }
}
