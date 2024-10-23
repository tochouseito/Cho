#pragma once

#include<cstdint>

class ResourceViewManager;
class SceneView
{
public:
	void Initialize(uint32_t index,ResourceViewManager* rvManager);

	void Update();
private:
	ResourceViewManager* rvManager_ = nullptr;

	uint32_t sceneTextureIndex;

};

