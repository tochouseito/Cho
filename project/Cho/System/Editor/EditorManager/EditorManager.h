#pragma once

// C++
#include<memory>

// 
#include"Editor/MainMenu/MainMenu.h"
#include"Editor/PopupMenu/PopupMenu.h"
#include"Editor/FileView/FileView.h"
#include"Editor/SceneView/SceneView.h"
#include"Editor/InfoView/InfoView.h"
#include"Editor/ObjectsList/ObjectsList.h"

// D3D12
class ResourceViewManager;
class RTVManager;
class DrawExecution;

// Loader
class TextureLoader;

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
		TextureLoader* texLoader,
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

	void SetSelectedGO(GameObject* gameObject) { selectedGameObject_ = gameObject; }
	void SetSelectedGOName(const std::string& name) { selectedGamaObjectName_ = name; }

	GameObject* GetSelectedGO()const { return selectedGameObject_; }
	std::string GetSelectedGOName()const { return selectedGamaObjectName_; }

private:// メンバ関数

	void UpdateMainWindow();

private:

	std::unique_ptr<MainMenu> mainMenu = nullptr;
	std::unique_ptr<PopupMenu> popupMenu = nullptr;
	std::unique_ptr<FileView> fileView = nullptr;
	std::unique_ptr<SceneView> sceneView = nullptr;
	std::unique_ptr<InfoView> infoView = nullptr;
	std::unique_ptr<ObjectsList> objectsList = nullptr;

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

