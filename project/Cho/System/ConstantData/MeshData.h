#pragma once

// C++
#include<cstdint>

// Utility
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"

// 頂点データ構造体
struct VertexData {
	Vector4 position = { 0.0f };
	Vector2 texcoord = { 0.0f };
	Vector3 normal = { 0.0f };
};
struct VertexSize {
	uint32_t vertices = 0;
	uint32_t indices = 0;
};
struct MeshData {
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	VertexSize size;
	uint32_t meshViewIndex = 0;
};
// スプライト用
struct SpriteVertexData {
	Vector2 position = { 0.0f };
	Vector2 texcoord = { 0.0f };
};
struct SpriteMeshData {
	SpriteVertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	VertexSize size;
	uint32_t meshViewIndex = 0;
};