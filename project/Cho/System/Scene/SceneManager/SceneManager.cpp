#include "PrecompiledHeader.h"
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

std::string SceneManager::AddGameObject(const std::string& objectName)
{
	std::string newName = GenerateUniqueName(gameObjects, objectName);
	// 新しいGameObjectを作成してマップに追加
	gameObjects[newName] = std::make_unique<GameObject>();
	GameObject* gameObject = gameObjects[newName].get();
	gameObject->SetManager(entityManager_, componentManager_);
	gameObject->CreateEntity();

	return newName;
}

std::string SceneManager::AddCameraObject(const std::string& cameraName)
{
	std::string newName = GenerateUniqueName(gameObjects, cameraName);
	// 新しいGameObjectを作成してマップに追加
	cameraObjects[newName] = std::make_unique<GameObject>();
	GameObject* cameraObject = cameraObjects[newName].get();
	cameraObject->SetManager(entityManager_, componentManager_);
	cameraObject->CreateCameraEntity();
	CameraComponent cameraComp;
	cameraComp.Initialize();
	cameraObject->AddComponent(cameraComp);

	return newName;
}

void SceneManager::CreateDebugCamera()
{
	debugCameraObject = std::make_unique<GameObject>();
	//std::string name = "DebugCamera";
	debugCameraObject->SetManager(entityManager_, componentManager_);
	debugCameraObject->CreateCameraEntity();
	CameraComponent cameraComp;
	cameraComp.Initialize();
	debugCameraObject->AddComponent(cameraComp);
}

// ユニークな名前を生成する関数
std::string SceneManager::GenerateUniqueName(
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& objects,
	const std::string& baseName) {
	std::string newName = baseName;
	int counter = 1;

	// 名前が重複している間、新しい名前に番号を追加してチェック
	while (objects.contains(newName)) {
		newName = baseName + " (" + std::to_string(counter) + ")";
		counter++;
	}

	return newName;
}
