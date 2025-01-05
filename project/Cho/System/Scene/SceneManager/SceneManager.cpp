#include "PrecompiledHeader.h"
#include "SceneManager.h"
#include <assert.h>
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"ECS/EntityManager/EntityManager.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"ECS/System/SystemManager/SystemManager.h"

void SceneManager::Initialize(
	ResourceViewManager* rvManager,
	EntityManager* entityManager,
	ComponentManager* componentManager,
	SystemManager* systemManager,
	PrefabManager* prefabManager,
	InputManager* inputManager
)
{
	// シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();

	rvManager_ = rvManager;

	// ECS
	entityManager_ = entityManager;
	componentManager_ = componentManager;
	systemManager_ = systemManager;
	prefabManager_ = prefabManager;

	inputManager_ = inputManager;
}

void SceneManager::Finalize()
{
	delete sceneFactory_;
	/*最後のシーンの終了と解放*/
	if (scene_) {
		scene_->Finalize();
	}
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
		systemManager_->Start(*entityManager_, *componentManager_);
	}
	if (!scene_) { return; }
	scene_->Update();
}

void SceneManager::Draw()
{
	if (!scene_) { return; }
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
	gameObject->SetName(newName);
	gameObject->SetManager(entityManager_, componentManager_);
	gameObject->CreateEntity();

	return newName;
}

std::string SceneManager::AddCameraObject(const std::string& cameraName)
{
	std::string newName = GenerateUniqueName(cameraObjects, cameraName);
	// 新しいCameraObjectを作成してマップに追加
	cameraObjects[newName] = std::make_unique<GameObject>();
	GameObject* cameraObject = cameraObjects[newName].get();
	cameraObject->SetManager(entityManager_, componentManager_);
	cameraObject->CreateCameraEntity();
	CameraComponent cameraComp;
	cameraObject->AddComponent(cameraComp);

	return newName;
}

std::string SceneManager::AddSpriteObject(const std::string& spriteName)
{
	std::string newName = GenerateUniqueName(spriteObjects, spriteName);
	// 新しいGameObjectを作成してマップに追加
	spriteObjects[newName] = std::make_unique<GameObject>();
	GameObject* spriteObject = spriteObjects[newName].get();
	spriteObject->SetName(newName);
	spriteObject->SetManager(entityManager_, componentManager_);
	spriteObject->CreateSpriteEntity();

	return newName;
}

std::string SceneManager::AddParticleObject(const std::string& particleName)
{
	std::string newName = GenerateUniqueName(particleObjects, particleName);
	// 新しいGameObjectを作成してマップに追加
	particleObjects[newName] = std::make_unique<GameObject>();
	GameObject* particleObject = particleObjects[newName].get();
	particleObject->SetName(newName);
	particleObject->SetManager(entityManager_, componentManager_);
	particleObject->CreateParticleEntity();

	return newName;
}

std::string SceneManager::GameObjectListRename(const std::string& newName, const std::string& deleteName)
{
	if (newName == deleteName) {
		return newName;
	}
	// 古い名前が存在するか確認
	auto it = gameObjects.find(deleteName);
	if (!gameObjects.contains(deleteName)) {
		//std::cerr << "Error: GameObject with name \"" << deleteName << "\" not found.\n";
		assert(0);
	}

	std::string NewName = GenerateUniqueName(gameObjects, newName);

	// オブジェクトを一時保存して、古い名前を削除
	auto object = std::move(it->second);
	gameObjects.erase(it);

	// 新しい名前で挿入
	gameObjects[NewName] = std::move(object);
	GameObject* gameObject = gameObjects[NewName].get();
	gameObject->SetName(NewName);

	return NewName;
}

std::string SceneManager::CameraObjectListRename(const std::string& newName, const std::string& deleteName)
{
	if (newName == deleteName) {
		return newName;
	}
	// 古い名前が存在するか確認
	auto it = cameraObjects.find(deleteName);
	if (!cameraObjects.contains(deleteName)) {
		//std::cerr << "Error: cameraObject with name \"" << deleteName << "\" not found.\n";
		assert(0);
	}

	std::string NewName = GenerateUniqueName(gameObjects, newName);

	// オブジェクトを一時保存して、古い名前を削除
	auto object = std::move(it->second);
	cameraObjects.erase(it);

	// 新しい名前で挿入
	cameraObjects[NewName] = std::move(object);

	return NewName;
}

std::string SceneManager::SpriteObjectListRename(const std::string& newName, const std::string& deleteName)
{
	if (newName == deleteName) {
		return newName;
	}
	// 古い名前が存在するか確認
	auto it = spriteObjects.find(deleteName);
	if (!spriteObjects.contains(deleteName)) {
		//std::cerr << "Error: GameObject with name \"" << deleteName << "\" not found.\n";
		assert(0);
	}

	std::string NewName = GenerateUniqueName(spriteObjects, newName);

	// オブジェクトを一時保存して、古い名前を削除
	auto object = std::move(it->second);
	spriteObjects.erase(it);

	// 新しい名前で挿入
	spriteObjects[NewName] = std::move(object);
	GameObject* gameObject = spriteObjects[NewName].get();
	gameObject->SetName(NewName);

	return NewName;
}

std::string SceneManager::ParticleObjectListRename(const std::string& newName, const std::string& deleteName)
{
	if (newName == deleteName) {
		return newName;
	}
	// 古い名前が存在するか確認
	auto it = particleObjects.find(deleteName);
	if (!particleObjects.contains(deleteName)) {
		//std::cerr << "Error: GameObject with name \"" << deleteName << "\" not found.\n";
		assert(0);
	}

	std::string NewName = GenerateUniqueName(particleObjects, newName);

	// オブジェクトを一時保存して、古い名前を削除
	auto object = std::move(it->second);
	particleObjects.erase(it);

	// 新しい名前で挿入
	particleObjects[NewName] = std::move(object);
	GameObject* gameObject = particleObjects[NewName].get();
	gameObject->SetName(NewName);

	return NewName;
}

// ユニークな名前を生成する関数
std::string SceneManager::GenerateUniqueName(
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& objects,
	const std::string& baseName) {
	std::string newName = baseName;
	int counter = 1;

	// 正規表現で "(番号)" を検出
	std::regex numberRegex(R"(^(.*) \((\d+)\)$)");
	std::smatch match;

	if (std::regex_match(baseName, match, numberRegex)) {
		// "baseName (番号)" の場合
		newName = match[1];                 // ベース名（括弧前の部分）
		counter = std::stoi(match[2]) + 1; // 現在の番号を取得して次から判定
	} else {
		// 括弧がない場合、すでに重複していないならそのまま返す
		if (!objects.contains(baseName)) {
			return baseName;
		}
	}

	// 名前が重複している間、新しい名前に番号を追加してチェック
	while (objects.contains(newName + " (" + std::to_string(counter) + ")")) {
		counter++;
	}

	return newName + " (" + std::to_string(counter) + ")";
}
