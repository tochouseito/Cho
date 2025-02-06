#include "PrecompiledHeader.h"
#include "MainCamera.h"
#include "Game/Scripts/Camera/CameraShake/CameraShake.h"

#include"Scene/SceneManager/SceneManager.h"

void MainCamera::Initialize()
{
	sceneManager_->SetNowCamera(sceneManager_->GetGameObject(name)->GetEntityID());
	camera->translation.y = 4.0f;
}

void MainCamera::Update()
{
	CameraShake::GetInstance()->Update();
	if (followPos_) {
		camera->translation = offsetPos_ + (*followPos_) + CameraShake::GetInstance()->GetShake();;
		camera->degrees = offsetDegrees_;
	}
}
