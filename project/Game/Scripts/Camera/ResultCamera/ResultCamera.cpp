#include "PrecompiledHeader.h"
#include "ResultCamera.h"
#include "Scene/SceneManager/SceneManager.h"

void ResultCamera::Initialize()
{
	sceneManager_->SetNowCamera(sceneManager_->GetGameObject(name)->GetEntityID());

	camera->translation = { 2.0f,7.0f,-22.0f };
	camera->degrees.x = 4.0f;
}

void ResultCamera::Update()
{
}
