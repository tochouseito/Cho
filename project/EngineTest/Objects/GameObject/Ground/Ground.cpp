#include "PrecompiledHeader.h"
#include "Ground.h"
#include"Scene/SceneManager/SceneManager.h"
#include"ECS/ComponentManager/ComponentManager.h"

void Ground::Init(SceneManager* sceneManager, ComponentManager* compManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;

	tf = compManager_->GetTransform(sceneManager_->GetGameObject(name)->GetEntityID());
	mesh = compManager_->GetMesh(sceneManager_->GetGameObject(name)->GetEntityID());
	material = compManager_->GetMaterial(sceneManager_->GetGameObject(name)->GetEntityID());

	tf->degrees.x = 270.0f;
	tf->scale.x = 100.0f;
	tf->scale.y = 100.0f;

	mesh->meshID = static_cast<uint32_t>(MeshPattern::Plane);

	material->textureID = textureID;
}

void Ground::Update()
{
}
