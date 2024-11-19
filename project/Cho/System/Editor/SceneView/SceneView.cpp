#include "PrecompiledHeader.h"
#include "SceneView.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// C++
#include<algorithm>

void SceneView::Initialize(uint32_t index, ResourceViewManager* rvManager)
{
	sceneTextureIndex = index;
	rvManager_ = rvManager;
}

void SceneView::Update(uint32_t cameraIndex)
{
    // シーンビュー（ゲームカメラ）のビュー
    GameView(cameraIndex);

    // デバッグ用ビュー
    DebugView();
}

void SceneView::DebugView()
{
    // ウィンドウのパディングをゼロに設定
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    // ウィンドウの開始
    ImGui::Begin("Debug View", nullptr, windowFlags);

    // ウィンドウ内で利用可能な領域のサイズを取得
    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    // テクスチャを描画する大きさを、利用可能な領域に合わせて設定
    ImVec2 textureSize = availableSize;

    // テクスチャを描画
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(sceneTextureIndex).GPUHandle;
    ImTextureID textureID = (ImTextureID)srvHandle.ptr;
    ImGui::Image(textureID, textureSize);

    // "Debug View" 上にカーソルがあるとき、右クリックメニューを開かないようにする
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)&&!excludeRightClickMenu) {
        excludeRightClickMenu = true;
    }

    ImGui::End();

    // スタイルを元に戻す
    ImGui::PopStyleVar();
}

void SceneView::GameView(uint32_t cameraIndex)
{
    // ウィンドウのパディングをゼロに設定
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    // ウィンドウの開始
    ImGui::Begin("Scene View", nullptr,windowFlags);

    // ウィンドウの位置とサイズを取得
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    // 固定解像度のテクスチャサイズ
    ImVec2 textureResolution = {
        static_cast<float>(WindowWidth()),
        static_cast<float>(WindowHeight())
    };

    // アスペクト比を維持しつつ、ウィンドウ内に収まるようにスケーリング係数を計算
    float scaleX = windowSize.x / textureResolution.x;
    float scaleY = windowSize.y / textureResolution.y;
    float scale = std::min(scaleX, scaleY);

    // スケーリング後のテクスチャサイズを計算
    ImVec2 scaledTextureSize = ImVec2(textureResolution.x * scale, textureResolution.y * scale);

    // 描画位置のオフセットを計算（ウィンドウの中央に配置するため）
    float offsetX = windowPos.x + (windowSize.x - scaledTextureSize.x) * 0.5f;
    float offsetY = windowPos.y + (windowSize.y - scaledTextureSize.y) * 0.5f;

    // カーソル位置を設定し、スクリーン上にテクスチャを中央に描画
    ImGui::SetCursorScreenPos(ImVec2(offsetX, offsetY));

    // テクスチャの描画
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(cameraIndex).GPUHandle;
    ImTextureID textureID = (ImTextureID)srvHandle.ptr;
    ImGui::Image(textureID, scaledTextureSize);

    // "Scene View" 上にカーソルがあるとき、右クリックメニューを開かないようにする
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        excludeRightClickMenu = true;
    } else
    {
        excludeRightClickMenu = false;
    }

    // ウィンドウの終了
    ImGui::End();

    // スタイルを元に戻す
    ImGui::PopStyleVar();
}
