#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class CameraSystem
{
public:

	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	// 更新
	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	void UpdateEditor(EntityManager& entityManager, ComponentManager& componentManager);
	// デバッグカメラ更新
	void UpdateDebugCamera(CameraComponent& camera);
private:
	void InitMatrix(CameraComponent& compo);

	// 行列更新
    void UpdateMatrix(CameraComponent& compo);
    void TransferMatrix(CameraComponent& compo);
};

