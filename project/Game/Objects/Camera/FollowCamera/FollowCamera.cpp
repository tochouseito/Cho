#include "PrecompiledHeader.h"
#include "FollowCamera.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"Scene/SceneManager/SceneManager.h"

void FollowCamera::Init(SceneManager* sceneManager, ComponentManager* compManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;

	comp = compManager_->GetCamera(sceneManager_->GetCameraObject(name)->GetEntityID());

	comp->translation.y = 10.0f;

}

void FollowCamera::Update()
{

}
