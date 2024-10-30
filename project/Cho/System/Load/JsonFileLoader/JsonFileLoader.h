#pragma once

// C++
#include <fstream>
#include <filesystem>

// Json
#include <json.hpp>

class JsonFileLoader
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	// ImGuiのスタイルを指定したファイルに保存する関数
	void SaveStyle(const char* filename);

	// 指定したファイルからImGuiのスタイルを読み込む関数
	void LoadStyle(const char* filename);

	// プロジェクトの「Resources/ImGuiStyle」ディレクトリにスタイルを保存する関数
	void SaveStyleToProject();

	// プロジェクトの「Resources/ImGuiStyle/style.json」からスタイルを読み込む関数
	void LoadStyleFromProject();

private:

};

