#pragma once

#include"ConstantData/MeshData.h"

// 数学ライブラリ
#include"ChoMath.h"

#include"ConstantData/WorldTransform.h"

// C++
#include<unordered_map>
#include<string>
#include<vector>

struct Node
{
	NodeTransform transform;
	Matrix4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct ObjectData {
	std::vector<VertexData> vertices;// 頂点リスト
	std::vector<uint32_t>indices;// Indexリスト
	uint32_t meshIndex;
	bool useTexture = false;
};

// モデルデータ構造体
struct ModelData {
	std::unordered_map<std::string, ObjectData> objects;
	std::vector<std::string> names;
	Node rootNode;
	bool isBone = false;
};