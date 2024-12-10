#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class TransformSystem
{
public:

	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

private:

    void UpdateMatrix(TransformComponent* tfComp);
    void TransferMatrix(TransformComponent* tfComp);
};

