#pragma once

#include"ECS/ComponentManager/ComponentManager.h"

class ScriptSystem
{
public:
	void Update(EntityManager& entityManager, ComponentManager& componentManager);
};

