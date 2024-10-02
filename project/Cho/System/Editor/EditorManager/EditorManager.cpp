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
	// MainMenu
	mainMenu->Update();

	// FileView
	fileView->Update();
}
