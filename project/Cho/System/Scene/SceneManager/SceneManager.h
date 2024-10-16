#pragma once
#include"Scene\SceneFactory\SceneFactory.h"

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
};

