#include "SceneView.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void SceneView::Initialize(uint32_t index, ResourceViewManager* rvManager)
{
	sceneTextureIndex = index;
	rvManager_ = rvManager;
}

void SceneView::Update()
{
	// シーンビューを表示する ImGui のウィンドウ
	ImGui::Begin("Scene View");

	// テクスチャを ImGui::Image() で表示
	ImTextureID textureID = (ImTextureID)rvManager_->GetHandle(2).GPUHandle.ptr;
	ImVec2 imageSize(1280, 720);  // シーンビューのサイズを指定
	ImGui::Image(textureID, imageSize);

	ImGui::End();
}
