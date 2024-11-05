#include "PrecompiledHeader.h"
#include "CameraSystem.h"

void CameraSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetCameraEntities()) {
		CameraComponent* cameraCompo = componentManager.GetCamera(entity);

		if (cameraCompo) {
			cameraCompo->UpdateMatrix();
		}
	}
}
