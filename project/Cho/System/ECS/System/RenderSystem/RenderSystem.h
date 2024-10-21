#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class D3DCommand;
class ResourceViewManager;
class RenderSystem{
public:
	void Render(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager
	);
};

