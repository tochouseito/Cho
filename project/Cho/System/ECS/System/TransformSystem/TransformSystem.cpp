#include "PrecompiledHeader.h"
#include "TransformSystem.h"

void TransformSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
    entityManager;
    componentManager;
}

void TransformSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		TransformComponent* tfComp = componentManager.GetTransform(entity);

		if (tfComp) {
			UpdateMatrix(tfComp);
		}
	}
}

void TransformSystem::UpdateMatrix(TransformComponent* tfComp) {
    
    tfComp->rotation = ChoMath::FromEulerAngles(tfComp->rot);
    tfComp->rotation.Normalize();
    tfComp->matWorld = ChoMath::MakeAffineMatrix(tfComp->scale, tfComp->rotation, tfComp->translation);

    TransferMatrix(tfComp);
}

void TransformSystem::TransferMatrix(TransformComponent* tfComp) {
    tfComp->constData->matWorld = tfComp->matWorld;
    tfComp->constData->worldInverse = Transpose(Matrix4::Inverse(tfComp->matWorld));
    tfComp->constData->rootNode = tfComp->rootMatrix;
}
