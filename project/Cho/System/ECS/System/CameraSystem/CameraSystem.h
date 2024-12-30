#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class CameraSystem
{
public:

	// 更新
	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	// 行列更新
    void UpdateMatrix(CameraComponent& compo);
    void TransferMatrix(CameraComponent& compo);

	// デバッグカメラ更新
	void UpdateDebugCamera(CameraComponent& camera);
};

