#pragma once

// C++
#include<string>
#include<memory>
#include <filesystem>
#include <vector>

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

	void LoadModel(const std::string& directoryPath, const fs::directory_entry& entry);

	/// <summary>
	/// 開始ロード
	/// </summary>
	void FirstResourceLoad(const std::string& directoryPath);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);
	Scale CalculateValue(const std::vector<KeyframeScale>& keyframes, float time);
private:

	void LoadModelFile(ModelData* modelData, const std::string& directoryPath, const fs::directory_entry& entry);

	void LoadAnimationFile(ModelData* modelData, const std::string& directoryPath, const fs::directory_entry& entry);

	Node ReadNode(aiNode* node);

	void CreateSkeleton(ModelData* modelData,const Node& rootNode);

	int32_t CreateJoint(
		const Node& node,
		const std::optional<int32_t>& parent,
		std::vector<Joint>& joints
	);

	void SkeletonUpdate(ModelData* modelData);

	void ApplyAnimation(ModelData* modelData, const uint32_t& animationIndex, float animationTime);

	void CreateSkinCluster(
		ModelData* modelData,
		const std::string& name,
		const ObjectData& objectData
	);

	void SkinClusterUpdate(ModelData* modelData);
private:
	// ポインタ
	ResourceViewManager* rvManager_ = nullptr;
	TextureLoader* texLoader_ = nullptr;
	MeshLoader* meshLoader_ = nullptr;
};