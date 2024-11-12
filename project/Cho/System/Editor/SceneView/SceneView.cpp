#include "PrecompiledHeader.h"
#include "SceneView.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// C++
#include<algorithm>
#include"MyMath/MyMath.h"

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

        // ウィンドウ全体のサイズを取得
        ImVec2 windowSize = ImGui::GetWindowSize();

        // 固定解像度のテクスチャサイズ
        ImVec2 textureResolution = {
            static_cast<float>(WindowWidth()),
            static_cast<float>(WindowHeight())
        };

        // ウィンドウの中心位置を計算
        ImVec2 windowCenter = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

        // テクスチャの描画開始位置を計算（テクスチャの中心がウィンドウの中心に来るように）
        float offsetX = windowCenter.x - (textureResolution.x * 0.5f);
        float offsetY = windowCenter.y - (textureResolution.y * 0.5f);

        // カーソル位置を設定し、テクスチャを中央に描画
        ImGui::SetCursorPos(ImVec2(offsetX, offsetY));

        // テクスチャを描画（解像度は変更しない）
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(CameraIndex).GPUHandle;
        ImTextureID textureID = (ImTextureID)srvHandle.ptr;
        ImGui::Image(textureID, textureResolution);

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
