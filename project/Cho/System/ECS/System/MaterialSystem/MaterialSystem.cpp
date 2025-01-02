#include "PrecompiledHeader.h"
#include "MaterialSystem.h"

void MaterialSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		MaterialComponent* comp = componentManager.GetMaterial(entity);

		if (comp) {
			UpdateMatrix(comp);
		}
	}
}

void MaterialSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		MaterialComponent* comp = componentManager.GetMaterial(entity);

		if (comp) {
			UpdateMatrix(comp);
		}
	}
}

void MaterialSystem::UpdateMatrix(MaterialComponent* comp)
{
	comp->matUV = MakeAffineMatrix(Vector3(comp->uvScale.x, comp->uvScale.y, 1.0f), Vector3(0.0f, 0.0f, comp->uvRot), Vector3(comp->uvPos.x, comp->uvPos.y, 0.0f));

	TransferMatrix(comp);
}

void MaterialSystem::TransferMatrix(MaterialComponent* comp)
{
	comp->constData->color = comp->color;
	comp->constData->enableLighting = comp->enableLighting;
	comp->constData->matUV = comp->matUV;
	comp->constData->shininess = comp->shininess;
}
