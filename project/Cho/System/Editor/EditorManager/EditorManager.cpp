#include "EditorManager.h"

// ImGui
#include"imgui.h"

void EditorManager::Initialize()
{
	// MainMenu
	mainMenu = std::make_unique<MainMenu>();
	mainMenu->Initialize();
}

void EditorManager::Update()
{
	// MainMenu
	mainMenu->Update();
}
