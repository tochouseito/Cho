#include "PrecompiledHeader.h"
#include "GameOverCamera.h"
#include "Scene/SceneManager/SceneManager.h"

void GameOverCamera::Initialize()
{
	sceneManager_->SetNowCamera(sceneManager_->GetGameObject(name)->GetEntityID());

	camera->translation = { 2.0f,7.0f,-22.0f };
	camera->degrees.x = 4.0f;
}

void GameOverCamera::Update()
{
}
