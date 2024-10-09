#include "EditorManager.h"

// ImGui
#include"imgui.h"

void EditorManager::Initialize()
{
	// MainMenu
	mainMenu = std::make_unique<MainMenu>();
	mainMenu->Initialize();

	// FileView
	fileView = std::make_unique<FileView>();
	fileView->Initialize();

}

void EditorManager::Update()
{
	// 全体のImGuiウィンドウ
	UpdateMainWindow();

	// MainMenu
	mainMenu->Update();

	// FileView
	fileView->Update();
}

void EditorManager::UpdateMainWindow()
{
	// ウィンドウの位置を指定する
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	// ウィンドウのサイズを指定する
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

	// フラグでウィンドウがフォーカスされても前面に持ってこないように設定
	ImGui::Begin(
		"Cho Editor",
		nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
	);
	// ウィンドウの中に表示するUI要素
	ImGui::End();
}
