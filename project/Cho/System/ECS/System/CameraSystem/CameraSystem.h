#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class CameraSystem
{
public:

	// 更新
	void Update(EntityManager& entityManager, ComponentManager& componentManager);
};
