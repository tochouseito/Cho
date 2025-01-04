#pragma once

// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class ParticleSystem
{
public:
	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	void SetRVManagerPtr(ResourceViewManager* rvManager);
	void SetD3DCommandPtr(D3DCommand* d3dCommand);
	void SetGraphicsSystemPtr(GraphicsSystem* graphicsSystem);
private:
	void InitMatrix(ParticleComponent* comp);

	// 更新
	void UpdateMatrix(ParticleComponent* particleComp,EmitterComponent* emitterComp);

	void TransferMatrix(ParticleComponent* comp);

private:
	D3DCommand* d3dCommand_ = nullptr;
	ResourceViewManager* rvManager_ = nullptr;
	GraphicsSystem* graphicsSystem_ = nullptr;
};

