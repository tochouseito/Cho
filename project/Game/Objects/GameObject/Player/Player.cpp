#include "PrecompiledHeader.h"
#include "Player.h"
#include"Scene/SceneManager/SceneManager.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void Player::Init(SceneManager* sceneManager, ComponentManager* compManager)
{
	compManager_ = compManager;

	sceneManager_ = sceneManager;
	compManager_ = compManager;

	tf = compManager_->GetTransform(sceneManager_->GetGameObject(name)->GetEntityID());
	mesh = compManager_->GetMesh(sceneManager_->GetGameObject(name)->GetEntityID());
	material = compManager_->GetMaterial(sceneManager_->GetGameObject(name)->GetEntityID());

	tf->translation.y = 5.0f;
	tf->scale *= 10.0f;

	std::string meshName = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->names[0];
	mesh->meshID = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->objects[meshName].meshIndex;
}

void Player::Update()
{

}

void Player::Input()
{
}
