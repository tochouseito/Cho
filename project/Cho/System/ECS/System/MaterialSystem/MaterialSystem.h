#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class MaterialSystem
{
public:
	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	private:
		// 更新
		void UpdateMatrix(MaterialComponent* comp);

		void TransferMatrix(MaterialComponent* comp);
};

