#pragma once
#include"Scene\SceneFactory\SceneFactory.h"
#include"ECS/GameObject/GameObject.h"

// C++
#include<string>
#include<unordered_map>
#include<memory>

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
	void AddGameObject(const std::string& objectName);

	void AddCameraObject(const std::string& cameraName);

	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetGameObjects() const {
		return gameObjects;
	}

	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetCameraObjects() const {
		return cameraObjects;
	}

	// デバッグ用
	const GameObject* GetDebugCamera(){
		std::string key = "DebugCamera";
		GameObject* result = cameraObjects[key].get();
		return result;
	}
private:
	/*今のシーン*/
	BaseScene* scene_ = nullptr;
	/*次のシーン*/
	BaseScene* nextScene_ = nullptr;
	/*シーンファクトリー*/
	AbstractSceneFactory* sceneFactory_ = nullptr;

	/*ECS*/
	EntityManager* entityManager_ = nullptr;
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;
	PrefabManager* prefabManager_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
	std::unordered_map<std::string, std::unique_ptr<GameObject>> cameraObjects;
	//std::unordered_map<std::string, std::unique_ptr<GameObject>> lightObjects;
};

