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
	std::string AddGameObject(const std::string& objectName);

	std::string AddCameraObject(const std::string& cameraName);

	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetGameObjects() const {
		return gameObjects;
	}

	// ひとつのゲームオブジェクトを取得
	GameObject* GetGameObject(const std::string& name) {
		GameObject* result = gameObjects[name].get();
		return result;
	}

	const std::unordered_map<std::string, std::unique_ptr<GameObject>>& GetCameraObjects() const {
		return cameraObjects;
	}

	// デバッグ用
	void CreateDebugCamera();
	const GameObject* GetDebugCamera(){
		GameObject* result = debugCameraObject.get();
		return result;
	}

	GameObject* GetCameraObjectData(const std::string& name) {
		GameObject* result = cameraObjects[name].get();
		return result;
	}

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

	/*ECS*/
	EntityManager* entityManager_ = nullptr;
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;
	PrefabManager* prefabManager_ = nullptr;
	
	std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
	std::unordered_map<std::string, std::unique_ptr<GameObject>> cameraObjects;
	//std::unordered_map<std::string, std::unique_ptr<GameObject>> lightObjects;

	std::unique_ptr<GameObject > debugCameraObject;// デバッグカメラ
};

