#include "PrecompiledHeader.h"
#include "DebugCamera.h"

#include"ECS/System/SystemManager/SystemManager.h"

void DebugCamera::Initialize(SystemManager* systemManager)
{
	systemManager_ = systemManager;
}

void DebugCamera::Update()
{
	systemManager_->DebugCameraUpdate(cameraComponent);
}
