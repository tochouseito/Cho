#pragma once

#include<cstdint>

class ResourceViewManager;
class SceneView
{
public:
	void Initialize(uint32_t index,ResourceViewManager* rvManager);

	void Update(uint32_t cameraIndex);

	bool IsWindowHovered()const { return excludeRightClickMenu; }

private:

	void DebugView();

	void GameView(uint32_t cameraIndex);
private:
	ResourceViewManager* rvManager_ = nullptr;

	uint32_t sceneTextureIndex;

	// このウィンドウの右クリックでのポップアップウィンドウを無効にするためのフラグ
	bool excludeRightClickMenu = false;

};

