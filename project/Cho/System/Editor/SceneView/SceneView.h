#pragma once

#include<cstdint>

class ResourceViewManager;
class SceneView
{
public:
	void Initialize(uint32_t index,ResourceViewManager* rvManager);

	void Update(uint32_t CameraIndex);
private:
	ResourceViewManager* rvManager_ = nullptr;

	uint32_t sceneTextureIndex;

};

