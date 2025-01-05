#include "PrecompiledHeader.h"
#include "GameScene.h"
#include"Scene/SceneManager/SceneManager.h"

void GameScene::Initialize()
{
	sceneManager_ = BaseScene::GetSceneManager();
	compManager_ = BaseScene::GetSceneManager()->GetCompPtr();

	player = std::make_unique<Player>();
	sceneManager_->AddGameObject(player->name);
	player->Init(compManager_);

	TransformComponent tf;
	sceneManager_->GetGameObject(player->name)->AddComponent(tf);
	MeshComponent mesh;
	sceneManager_->GetGameObject(player->name)->AddComponent(mesh);
	MaterialComponent material;
	sceneManager_->GetGameObject(player->name)->AddComponent(material);
	RenderComponent render;
	sceneManager_->GetGameObject(player->name)->AddComponent(render);

}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	player->Update();
}

void GameScene::Draw()
{
}

void GameScene::ChangeScene()
{
}
