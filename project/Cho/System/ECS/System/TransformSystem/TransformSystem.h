#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class TransformSystem
{
public:

	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	// Editor更新
	void UpdateEditor(EntityManager& entityManager, ComponentManager& componentManager);

private:

	// 初期化
	void InitMatrix(TransformComponent* tfComp);

	// 更新
    void UpdateMatrix(TransformComponent* tfComp);

    void TransferMatrix(TransformComponent* tfComp);
};

