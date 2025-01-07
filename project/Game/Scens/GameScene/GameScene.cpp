#include "PrecompiledHeader.h"
#include "GameScene.h"
#include"Scene/SceneManager/SceneManager.h"

void GameScene::Initialize()
{
	sceneManager_ = BaseScene::GetSceneManager();
	compManager_ = BaseScene::GetSceneManager()->GetCompPtr();
	{
		player = std::make_unique<Player>();
		CreateGameObject(player->name);
		AnimationComponent animation;
		compManager_->AddComponent(sceneManager_->GetGameObject(player->name)->GetEntityID(), animation);
		player->Init(sceneManager_, compManager_,sceneManager_->GetInputManagerPtr());
	}
	{
		ground = std::make_unique<Ground>();
		CreateGameObject(ground->name);
		ground->Init(sceneManager_,compManager_);
	}
	{
		followCamera = std::make_unique<FollowCamera>();
		CreateCamera(followCamera->name);
		followCamera->Init(sceneManager_, compManager_,sceneManager_->GetInputManagerPtr());
		sceneManager_->SetNowCamera(sceneManager_->GetCameraObject(followCamera->name)->GetEntityID());
	}
	{
		for (int i = 0; i < 10; i++)
		{
			enemies.push_back(std::make_unique<Enemy>());

			enemies[i]->name=CreateGameObject(enemies[i]->name);
			enemies[i]->Init(sceneManager_, compManager_, sceneManager_->GetInputManagerPtr());
		}
	}
	// CollisionManager
	collisionManager = std::make_unique<CollisionManager>();
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	player->Update();
	followCamera->Update();
	
}

void GameScene::Draw()
{
}

void GameScene::ChangeScene()
{
}

std::string GameScene::CreateGameObject(const std::string& name)
{
	std::string newName;
	newName=sceneManager_->AddGameObject(name);
	TransformComponent tf;
	sceneManager_->GetGameObject(newName)->AddComponent(tf);
	MeshComponent mesh;
	sceneManager_->GetGameObject(newName)->AddComponent(mesh);
	MaterialComponent material;
	sceneManager_->GetGameObject(newName)->AddComponent(material);
	RenderComponent render;
	sceneManager_->GetGameObject(newName)->AddComponent(render);

	return newName;
}

void GameScene::CreateCamera(const std::string& name)
{
	sceneManager_->AddCameraObject(name);
	CameraComponent camera;
	sceneManager_->GetCameraObject(name)->AddComponent(camera);
}

void GameScene::CheckAllCollisions()
{
	// 衝突マネージャのリセット
	collisionManager->Reset();

	// コライダーをリストに登録
	collisionManager->AddCollider(player.get());
	// 敵すべてについて
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		collisionManager->AddCollider(enemy.get());
	}
	// 衝突判定と応答
	collisionManager->CheckAllCollisions();
}
