#pragma once

// C++
#include<memory>

// Entity
#include"Editor/MainMenu/MainMenu.h"
#include"Editor/FileView/FileView.h"

class EditorManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private:

	std::unique_ptr<MainMenu> mainMenu = nullptr;
	std::unique_ptr<FileView> fileView = nullptr;
};

