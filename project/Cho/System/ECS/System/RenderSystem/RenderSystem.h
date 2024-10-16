#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class RenderSystem{
public:
	void Render(EntityManager& entityManager, ComponentManager& componentManager);
};

