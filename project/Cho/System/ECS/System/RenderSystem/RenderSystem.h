#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class TextureLoader;
class RenderSystem{
public:
	void Render(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager,
		GraphicsSystem* graphicsSystem,
		TextureLoader* texLoad
	);

	void DebugRender(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager,
		GraphicsSystem* graphicsSystem,
		TextureLoader* texLoad,
		const CameraComponent& camera
		);

private:
	void ObjectRender(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager,
		GraphicsSystem* graphicsSystem,
		TextureLoader* texLoad
	);

	void SpriteRender(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager,
		GraphicsSystem* graphicsSystem,
		TextureLoader* texLoad
	);

	void ParticleRender(
		EntityManager& entityManager,
		ComponentManager& componentManager,
		D3DCommand* d3dCommand,
		ResourceViewManager* rvManager,
		GraphicsSystem* graphicsSystem,
		TextureLoader* texLoad
	);
};

