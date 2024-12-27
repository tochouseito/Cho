#pragma once

// C++
#include<string>
#include<memory>

// assimp
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

// モデルデータ構造体
#include"ConstantData/ModelData.h"

class TextureLoader;
class MeshLoader;
class ModelLoader
{
public:

	void Initialize(TextureLoader* texLoader,MeshLoader* meshLoader);

	void LoadModel(const std::string& fileRoot, const std::string& fileName);

private:
	ModelData* LoadModelFile(const std::string& fileRoot, const std::string& fileName);
private:
	// ポインタ
	TextureLoader* texLoader_ = nullptr;
	MeshLoader* meshLoader_ = nullptr;

	// Modelコンテナ
	std::unordered_map<std::string, std::unique_ptr<ModelData>> modelContainer;
};