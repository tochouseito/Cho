#pragma once
// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class D3DCommand;
class ResourceViewManager;
class GraphicsSystem;
class AnimationSystem
{
public:
	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	void SetRVManagerPtr(ResourceViewManager* rvManager) { rvManager_ = rvManager; }
	void SetD3DCommandPtr(D3DCommand* d3dCommand) { d3dCommand_ = d3dCommand; }
	void SetGraphicsSystemPtr(GraphicsSystem* graphicsSystem) { graphicsSystem_ = graphicsSystem; }

private:

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);
	Scale CalculateValue(const std::vector<KeyframeScale>& keyframes, float time);

	void timeUpdate(AnimationComponent* comp,ModelData* model);

	void ApplyAnimation(AnimationComponent* comp, ModelData* model);
	void SkeletonUpdate(AnimationComponent* comp, ModelData* model);
	void SkinClusterUpdate(AnimationComponent* comp, ModelData* model);

	void ApplySkinning(AnimationComponent* comp, ModelData* model);
private:
	D3DCommand* d3dCommand_ = nullptr;
	ResourceViewManager* rvManager_ = nullptr;
	GraphicsSystem* graphicsSystem_ = nullptr;
};

