#include "PrecompiledHeader.h"
#include "CameraSystem.h"

void CameraSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetCameraEntities()) {
		CameraComponent* cameraCompo = componentManager.GetCamera(entity);

		if (cameraCompo) {
			UpdateMatrix(*cameraCompo);
		}
	}
}

void CameraSystem::UpdateMatrix(CameraComponent& compo) {
	Quaternion rotate = FromEulerAngles(compo.rotation);
	compo.matWorld = MakeAffineMatrix(Scale(1.0f, 1.0f, 1.0f), rotate, compo.translation);

	TransferMatrix(compo);
}

void CameraSystem::TransferMatrix(CameraComponent& compo) {
	compo.constData->matWorld = compo.matWorld;
	compo.constData->view = Matrix4::Inverse(compo.matWorld);
	//float width = static_cast<float>(WindowWidth());
	//float height = static_cast<float>(WindowHeight());
	compo.constData->projection = MakePerspectiveFovMatrix(
		compo.fovAngleY, compo.aspectRatio, compo.nearZ, compo.farZ
	);
	compo.constData->cameraPosition = compo.translation;
}

void CameraSystem::UpdateDebugCamera(CameraComponent& camera)
{
	UpdateMatrix(camera);
}
