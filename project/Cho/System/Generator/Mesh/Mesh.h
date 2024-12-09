#pragma once

// C++
#include<numbers>
#include<vector>
#include<unordered_map>

// Utility
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"

// 頂点データ構造体（テクスチャあり）
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct VertexSize {
	uint32_t vertices;
	uint32_t indices;
};
struct MeshData {
	VertexData* vertexData;
	uint32_t* indexData = nullptr;
	VertexSize size;
	uint32_t meshViewIndex;
};
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
class Mesh
{
public:
	static Meshs GeneratorMeshs(MeshPattern pattern, ResourceViewManager* rvManager);
private:
	// 球体生成
	static VertexSize CreateSphere(MeshData& data, ResourceViewManager* rvManager);

	// 立方体生成
	static VertexSize CreateCube(MeshData& data, ResourceViewManager* rvManager);

	// 平面生成
	static VertexSize CreatePlane(MeshData& data, ResourceViewManager* rvManager);
};

