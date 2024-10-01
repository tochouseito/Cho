#include "ImGuiManager.h"
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

#include"WinApp/WinApp.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void ImGuiManager::Initialize(WinApp* win, D3DDevice* d3dDevice, ResourceViewManager* RVManager)
{
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(win->GetHwnd());
	ImGui_ImplDX12_Init(
		d3dDevice->GetDevice(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM, RVManager->GetDescriptorHeap(),
		RVManager->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		RVManager->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());


	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Dockingを有効化

	// フォントファイルのパスとサイズを指定してフォントをロードする
	io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\Arial.ttf", 16.0f // フォントファイルのパスとフォントサイズ
	);
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw(ID3D12GraphicsCommandList* commandList)
{
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
