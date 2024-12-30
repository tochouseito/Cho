#include "PrecompiledHeader.h"
#include "DebugCamera.h"

#include"ECS/System/SystemManager/SystemManager.h"

void DebugCamera::Initialize(ComponentManager* componentManager,SystemManager* systemManager)
{
	componentManager_ = componentManager;
	systemManager_ = systemManager;

	componentManager->InitCameraComponent(&cameraComponent);
}

void DebugCamera::Update()
{
	systemManager_->DebugCameraUpdate(cameraComponent);
}
