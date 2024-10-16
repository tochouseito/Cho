#include "SceneManager.h"
#include<assert.h>

void SceneManager::Initialize(
	EntityManager* entityManager,
	ComponentManager* componentManager,
	SystemManager* systemManager,
	PrefabManager* prefabManager
)
{
	// シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();

	// ECS
	entityManager_ = entityManager;
	componentManager_ = componentManager;
	systemManager_ = systemManager;
	prefabManager_ = prefabManager;
}

void SceneManager::Finalize()
{
	delete sceneFactory_;
	/*最後のシーンの終了と解放*/
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	if (nextScene_) {
		/*旧シーンの終了*/
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		/*シーン切り替え*/
		scene_ = nextScene_;
		nextScene_ = nullptr;
		/*シーンマネージャをセット*/
		scene_->SetSceneManager(this);
		/*次のシーンを初期化する*/
		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	/*次のシーン生成*/
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
