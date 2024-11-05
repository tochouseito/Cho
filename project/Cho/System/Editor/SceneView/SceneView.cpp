#include "PrecompiledHeader.h"
#include "SceneView.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void SceneView::Initialize(uint32_t index, ResourceViewManager* rvManager)
{
	sceneTextureIndex = index;
	rvManager_ = rvManager;
}

void SceneView::Update(uint32_t CameraIndex)
{
	{
		// ウィンドウの開始
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// ウィンドウ内で利用可能な領域のサイズを取得
		ImVec2 availableSize = ImGui::GetContentRegionAvail();

		// テクスチャを描画する大きさを、利用可能な領域に合わせて設定
		ImVec2 textureSize = availableSize;

		// テクスチャを描画
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(CameraIndex).GPUHandle;
		ImTextureID textureID = (ImTextureID)srvHandle.ptr;
		ImGui::Image(textureID, textureSize);

		ImGui::End();
	}

	{
		// ウィンドウの開始
		ImGui::Begin("Debug View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// ウィンドウ内で利用可能な領域のサイズを取得
		ImVec2 availableSize = ImGui::GetContentRegionAvail();

		// テクスチャを描画する大きさを、利用可能な領域に合わせて設定
		ImVec2 textureSize = availableSize;

		// テクスチャを描画
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(sceneTextureIndex).GPUHandle;
		ImTextureID textureID = (ImTextureID)srvHandle.ptr;
		ImGui::Image(textureID, textureSize);

		ImGui::End();
	}
}
