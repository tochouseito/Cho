#include "PrecompiledHeader.h"
#include "SceneView.h"

#include"imgui.h"

#include"WinApp/WinApp.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Editor/DebugCamera/DebugCamera.h"
#include"Input/InputManager.h"
#include"SystemState/SystemState.h"

// C++
#include<algorithm>

void SceneView::Initialize(const uint32_t& index,WinApp* win, ResourceViewManager* rvManager, InputManager* inputManager, DebugCamera* debugCamera)
{
	sceneTextureIndex = index;
    win_ = win;
	rvManager_ = rvManager;
	inputManager_ = inputManager;
    debugCamera_ = debugCamera;
}

void SceneView::Update(const uint32_t& cameraIndex)
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

	ImVec2 windowPos = ImGui::GetWindowPos(); // 現在のImGuiウィンドウの位置を取得
	float diffY = ImGui::GetWindowSize().y- ImGui::GetContentRegionAvail().y;
	windowPos.y += diffY;
	ImVec2 windowSize = ImGui::GetContentRegionAvail(); // 現在のImGuiウィンドウのサイズを取得

    Vector2 vec = CheckAndWarpMouseInImGuiWindow(Vector2(windowPos.x, windowPos.y), Vector2(windowSize.x, windowSize.y));
    /*if (vec.x != 0.0f || vec.y != 0.0f) {
        
    }*/
    debugCamera_->SetCurrentMousePos(vec);
    // アスペクト比を計算してカメラに設定
    float newAspect = availableSize.x / availableSize.y;
    debugCamera_->SetAspect(newAspect);

    // テクスチャを描画
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(sceneTextureIndex).GPUHandle;
    ImTextureID textureID = (ImTextureID)srvHandle.ptr;
    ImGui::Image(textureID, availableSize);

    // "Debug View" 上にカーソルがあるとき、右クリックメニューを開かないようにする
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)&&!excludeRightClickMenu) {
        excludeRightClickMenu = true;
        if (inputManager_->IsPressMouse(Left) ||
            inputManager_->IsPressMouse(Right) ||
            inputManager_->IsPressMouse(Center)) 
        {
			windowClick = true;
        }
        else {
			windowClick = false;
            setMousePos = true;
        }
    }
    else {
        excludeRightClickMenu = false;
    }

    ImGui::End();

    // スタイルを元に戻す
    ImGui::PopStyleVar();
}

void SceneView::GameView(const uint32_t& cameraIndex)
{
    // ウィンドウのパディングをゼロに設定
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    // ウィンドウの開始
    ImGui::Begin("Scene View", nullptr, windowFlags);

    // ウィンドウ内の描画可能領域を取得
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();

    // 固定解像度のテクスチャサイズ
    ImVec2 textureResolution = {
        static_cast<float>(WindowWidth()),
        static_cast<float>(WindowHeight())
    };

    // アスペクト比を維持しつつ、ウィンドウ内の描画可能領域に収まるようにスケーリング係数を計算
    float scaleX = contentRegion.x / textureResolution.x;
    float scaleY = contentRegion.y / textureResolution.y;
    float scale = std::min(scaleX, scaleY);

    // スケーリング後のテクスチャサイズを計算
    ImVec2 scaledTextureSize = ImVec2(textureResolution.x * scale, textureResolution.y * scale);

    // 描画位置のオフセットを計算（ウィンドウの中央に配置するため）
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    float offsetX = cursorPos.x + (contentRegion.x - scaledTextureSize.x) * 0.5f;
    float offsetY = cursorPos.y + (contentRegion.y - scaledTextureSize.y) * 0.5f;

    // カーソル位置を設定し、スクリーン上にテクスチャを中央に描画
    ImGui::SetCursorScreenPos(ImVec2(offsetX, offsetY));

    // テクスチャの描画
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = rvManager_->GetHandle(cameraIndex).GPUHandle;
    ImTextureID textureID = (ImTextureID)srvHandle.ptr;
    ImGui::Image(textureID, scaledTextureSize);

    // "Scene View" 上にカーソルがあるとき、右クリックメニューを開かないようにする
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        excludeRightClickMenu = true;
    }
    else {
        excludeRightClickMenu = false;
    }

    // ウィンドウの終了
    ImGui::End();

    // スタイルを元に戻す
    ImGui::PopStyleVar();
}

Vector2 SceneView::CheckAndWarpMouseInImGuiWindow(const Vector2& pos, const Vector2& size)
{
    if (windowClick) {

        // ImGuiウィンドウのスクリーン座標を取得
        POINT windowTopLeft = { static_cast<LONG>(pos.x), static_cast<LONG>(pos.y) };
        POINT windowBottomRight = { static_cast<LONG>(pos.x + size.x), static_cast<LONG>(pos.y + size.y) };

        // クライアント座標をスクリーン座標に変換
        ClientToScreen(win_->GetHwnd(), &windowTopLeft);
        ClientToScreen(win_->GetHwnd(), &windowBottomRight);

        // 変換後の座標を取得
        int screenLeft = windowTopLeft.x;
        int screenTop = windowTopLeft.y;
        int screenRight = windowBottomRight.x;
        int screenBottom = windowBottomRight.y;

        // マウスカーソルの現在位置を取得
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        // ワープ後のマウス位置を保存
        static POINT prevCursorPos = cursorPos; // 前フレームの位置を保持

		if (setMousePos) {
            GetCursorPos(&prevCursorPos);
			setMousePos = false;
		}

        // マウスの相対移動量を計算
        float deltaX = static_cast<float>(cursorPos.x - prevCursorPos.x);
        float deltaY = static_cast<float>(cursorPos.y - prevCursorPos.y);

        // ワープ処理
        if (cursorPos.x <= screenLeft) {
            SetCursorPos(screenRight - 2, cursorPos.y);
            deltaX += static_cast<float>((screenRight - 2) - cursorPos.x);
        }
        else if (cursorPos.x >= screenRight - 1) {
            SetCursorPos(screenLeft + 2, cursorPos.y);
            deltaX -= static_cast<float>(cursorPos.x - (screenLeft + 2));
        }

        if (cursorPos.y <= screenTop) {
            SetCursorPos(cursorPos.x, screenBottom - 2);
            deltaY += static_cast<float>((screenBottom - 2) - cursorPos.y);
        }
        else if (cursorPos.y >= screenBottom - 1) {
            SetCursorPos(cursorPos.x, screenTop + 2);
            deltaY -= static_cast<float>(cursorPos.y - (screenTop + 2));
        }

        // 次フレームのために現在のマウス位置を保存
        GetCursorPos(&prevCursorPos);

        //// マウスの相対移動量（補正用）
        //float deltaX = 0.0f;
        //float deltaY = 0.0f;

        //// ImGuiウィンドウ内でワープ処理
        //if (cursorPos.x <= screenLeft) {
        //    deltaX = static_cast<float>(screenRight - 2); // ワープによる移動量
        //    SetCursorPos(screenRight - 2, cursorPos.y);
        //}
        //else if (cursorPos.x >= screenRight - 1) {
        //    deltaX = static_cast<float>(-screenRight + 2); // ワープによる移動量
        //    SetCursorPos(screenLeft + 2, cursorPos.y);
        //}

        //if (cursorPos.y <= screenTop) {
        //    deltaY = static_cast<float>(screenBottom - 2);
        //    SetCursorPos(cursorPos.x, screenBottom - 2);
        //}
        //else if (cursorPos.y >= screenBottom - 1) {
        //    deltaY = static_cast<float>(-screenBottom + 2);
        //    SetCursorPos(cursorPos.x, screenTop + 2);
        //}

        // マウスの相対移動量を補正
        Vector2 result = { deltaX,deltaY };
        return result;
    }
	return { 0.0f,0.0f };
}



