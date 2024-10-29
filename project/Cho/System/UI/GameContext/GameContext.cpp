#include "GameContext.h"
#include"imgui.h"


void GameContext::Initialize()
{

}

void GameContext::Update()
{
	// ImGuiでフレームレートとデルタタイムを表示
	/*ImGui::Begin("Framerate Info");
	ImGui::Text("DeltaTime: %.6f", deltaTime_);
	ImGui::Text("FrameRate: %.1f FPS", frameRate_);
	ImGui::Text("FrameRate: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();*/
}

void GameContext::SetFrameValue(float framerate, float deltaTime)
{
	frameRate_ = framerate;

	deltaTime_ = deltaTime;
}
