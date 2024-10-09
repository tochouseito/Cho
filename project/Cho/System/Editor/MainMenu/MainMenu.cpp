#include "MainMenu.h"
#include"imgui.h"

void MainMenu::Initialize()
{

}

void MainMenu::Update()
{
	ImGui::Begin("MainMenu");

	// スタイルエディタを表示
	ImGui::ShowStyleEditor();

	ImGui::End();

	ImGui::Begin("AddObject");

	if (ImGui::Button("Cube")) {

	}

	ImGui::End();
}
