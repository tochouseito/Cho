#pragma once

// C++
#include<string>

// assimp
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

// モデルデータ構造体
#include"ConstantData/ModelData.h"

class ModelLoader
{
public:
	void LoadModelFile(const std::string& fileRoot, const std::string& fileName);

private:
	// Modelコンテナ
	std::unordered_map<std::string, ModelData> modelContainer;
};