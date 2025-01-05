#pragma once
#include"Scene\SceneFactory\SceneFactory.h"
#include"ECS/GameObject/GameObject.h"

// C++
#include<string>
#include<unordered_map>
#include<memory>
#include <regex>

class ResourceViewManager;

// ECS
class EntityManager;
class ComponentManager;
class SystemManager;
class PrefabManager;

class SceneManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
		ResourceViewManager* rvManager,
		EntityManager* entityManager,
		ComponentManager* componentManager,
		SystemManager* systemManager,
		PrefabManager* prefabManager
	);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/*シーン切り替え*/
	void ChangeScene(const std::string& sceneName);

public:
	std::string AddGameObject(const std::string& objectName);

	std::string AddCameraObject(const std::string& cameraName);

	std::string AddSpriteObject(const std::string& spriteName);

	std::string AddParticleObject(const std::string& particleName);

	// ベースオブジェクト
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetGameObjects() const {
		return gameObjects;
	}

	// ひとつのゲームオブジェクトを取得
	GameObject* GetGameObject(const std::string& name) {
		GameObject* result = gameObjects[name].get();
		return result;
	}

	// カメラ
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetCameraObjects() const {
		return cameraObjects;
	}

	GameObject* GetCameraObject(const std::string& name) {
		GameObject* result = cameraObjects[name].get();
		return result;
	}

	// スプライト
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetSpriteObjects() const{
		return spriteObjects;
	}

	GameObject* GetSpriteObject(const std::string& name) {
		GameObject* result = spriteObjects[name].get();
		return result;
	}

	// パーティクル
	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetParticleObjects() const {
		return particleObjects;
	}

	GameObject* GetParticleObject(const std::string& name) {
		GameObject* result = particleObjects[name].get();
		return result;
	}

	std::string GameObjectListRename(const std::string& newName, const std::string& deleteName);
	std::string CameraObjectListRename(const std::string& newName, const std::string& deleteName);
	std::string SpriteObjectListRename(const std::string& newName, const std::string& deleteName);
	std::string ParticleObjectListRename(const std::string& newName, const std::string& deleteName);

	ComponentManager* GetCompPtr()const { return componentManager_; }
	ResourceViewManager* GetRVManagerPtr()const { return rvManager_; }

	void SetNowCamera(const Entity& id) { entityManager_->SetCameraID(id); }

private:
	// ユニークな名前を生成する関数
	std::string GenerateUniqueName(
		const std::unordered_map<std::string, std::unique_ptr<GameObject>>& objects,
		const std::string& baseName
	);
private:
	/*今のシーン*/
	BaseScene* scene_ = nullptr;
	/*次のシーン*/
	BaseScene* nextScene_ = nullptr;
	/*シーンファクトリー*/
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// ResourceManager
	ResourceViewManager* rvManager_ = nullptr;

	/*ECS*/
	EntityManager* entityManager_ = nullptr;
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;
	PrefabManager* prefabManager_ = nullptr;
	
	std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
	std::unordered_map<std::string, std::unique_ptr<GameObject>> cameraObjects;
	//std::unordered_map<std::string, std::unique_ptr<GameObject>> lightObjects;
	std::unordered_map<std::string, std::unique_ptr<GameObject>> spriteObjects;
	std::unordered_map<std::string, std::unique_ptr<GameObject>> particleObjects;
};

