#pragma once

// D3D12
class ResourceViewManager;
class RTVManager;
class DrawExecution;

// ECS
class EntityManager;
class ComponentManager;
class SystemManager;
class PrefabManager;

// GameObject
class GameObject;

// SceneManager
class SceneManager;

enum PopType
{
	None,
	BaseObject,
};

class MainMenu
{
public:
	~MainMenu()=default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
		ResourceViewManager* rvManager,
		RTVManager* rtvManager,
		DrawExecution* drawExe,
		EntityManager* entityManager,
		ComponentManager* componentManager,
		SystemManager* systemManager,
		PrefabManager* prefabManager,
		SceneManager* sceneManager
	);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

	void BackWindow();

	void MenuBar();

	void FileMenu();

	void EditMenu();

	void LayoutMenu();

	void EngineInfoMenu();

	void Add();

	void PopUp();

private:
	/*D3D12*/
	ResourceViewManager* rvManager_ = nullptr;
	RTVManager* rtvManager_ = nullptr;
	DrawExecution* drawExe_ = nullptr;

	/*ECS*/
	EntityManager* entityManager_ = nullptr;
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;
	PrefabManager* prefabManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;

	PopType popWindow = PopType::None;
};

