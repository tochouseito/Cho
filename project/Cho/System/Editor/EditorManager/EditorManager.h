#pragma once

// C++
#include<memory>

// 
#include"Editor/MainMenu/MainMenu.h"
#include"Editor/FileView/FileView.h"
#include"Editor/SceneView/SceneView.h"

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

class EditorManager
{
public:

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

private:// メンバ関数

	void UpdateMainWindow();

private:

	std::unique_ptr<MainMenu> mainMenu = nullptr;
	std::unique_ptr<FileView> fileView = nullptr;
	std::unique_ptr<SceneView> sceneView = nullptr;

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

	std::string selectedGamaObjectName_;
	GameObject* selectedGameObject_ = nullptr;
};

