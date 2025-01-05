#pragma once

// C++
#include<string>
#include<memory>
#include <filesystem>

namespace fs = std::filesystem;

// assimp
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

// モデルデータ構造体
#include"ConstantData/ModelData.h"

class ResourceViewManager;
class TextureLoader;
class MeshLoader;
class ModelLoader
{
public:

	void Initialize(ResourceViewManager* rvManager,TextureLoader* texLoader, MeshLoader* meshLoader);

	void LoadModel(const std::string& fileRoot, const std::string& fileName);

	/// <summary>
	/// 開始ロード
	/// </summary>
	void FirstResourceLoad(const std::string& directoryPath);

private:

	void LoadModelFile(ModelData* modelData,const std::string& fileRoot, const std::string& fileName);

private:
	// ポインタ
	ResourceViewManager* rvManager_ = nullptr;
	TextureLoader* texLoader_ = nullptr;
	MeshLoader* meshLoader_ = nullptr;
};