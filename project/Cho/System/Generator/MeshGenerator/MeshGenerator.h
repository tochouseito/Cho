#pragma once

// C++
#include<numbers>
#include<vector>
#include<unordered_map>

// MeshData
#include"ConstantData/MeshData.h"
#include"ConstantData/SpriteData.h"

struct Meshs {
	std::unordered_map <std::string, MeshData> meshData;
	std::vector<std::string> names;
};
// Mesh
enum class MeshPattern:uint32_t
{
	Cube,
	Plane,
	Sphere,
	CountPattern,// カウント用
};
class ResourceViewManager;
class MeshGenerator
{
public:
	static Meshs GeneratorMeshs(MeshPattern pattern, ResourceViewManager* rvManager);

	// スプライト生成
	static void CreateSprite(SpriteMeshData& data, ResourceViewManager* rvManager);
private:
	// 球体生成
	static VertexSize CreateSphere(MeshData& data, ResourceViewManager* rvManager);

	// 立方体生成
	static VertexSize CreateCube(MeshData& data, ResourceViewManager* rvManager);

	// 平面生成
	static VertexSize CreatePlane(MeshData& data, ResourceViewManager* rvManager);
};

