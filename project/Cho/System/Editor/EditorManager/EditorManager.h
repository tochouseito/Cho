#pragma once

// C++
#include<memory>

// 
#include"Editor/MainMenu/MainMenu.h"
#include"Editor/FileView/FileView.h"

// ECS
class EntityManager;
class ComponentManager;
class SystemManager;
class PrefabManager;

class EditorManager
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
	/// 更新
	/// </summary>
	void Update();

private:// メンバ関数

	void UpdateMainWindow();

private:

	std::unique_ptr<MainMenu> mainMenu = nullptr;
	std::unique_ptr<FileView> fileView = nullptr;

	/*ECS*/
	EntityManager* entityManager_ = nullptr;
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;
	PrefabManager* prefabManager_ = nullptr;
};

