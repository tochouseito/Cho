#pragma once

#include<cstdint>

class DebugCamera;
class ResourceViewManager;
class SceneView
{
public:
	void Initialize(const uint32_t& index,ResourceViewManager* rvManager,DebugCamera* debugCamera);

	void Update(const uint32_t& cameraIndex);

	bool IsWindowHovered()const { return excludeRightClickMenu; }

private:

	void DebugView();

	void GameView(const uint32_t& cameraIndex);
private:
	// ポインタ
	ResourceViewManager* rvManager_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	uint32_t sceneTextureIndex;

	// このウィンドウの右クリックでのポップアップウィンドウを無効にするためのフラグ
	bool excludeRightClickMenu = false;

};

