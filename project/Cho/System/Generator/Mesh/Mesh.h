#pragma once

// Utility
#include"Base/Vector2.h"
#include"Base/Vector3.h"
#include"Base/Vector4.h"

// 頂点データ構造体（テクスチャあり）
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
enum class Object {
	kBox,
	kPlane,
	kSphere,
};
struct MeshData {
	VertexData* vertexData;
	uint32_t vertices;
	uint32_t vbvIndex;
};
class Mesh
{
public:
	
private:

};

