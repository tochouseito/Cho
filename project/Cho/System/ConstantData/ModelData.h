#pragma once

#include"ConstantData/MeshData.h"

// 数学ライブラリ
#include"ChoMath.h"

// デバッグ用
#include"ConstantData/DebugTransform.h"

// C++
#include<unordered_map>
#include<string>
#include<vector>

struct Node
{
	DebugTransform transform;
	Matrix4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct ObjectData {
	std::vector<VertexData> vertices;// 頂点数
	// マテリアルいるかも
	Node rootNode;
};

// モデルデータ構造体
struct ModelData {
	std::unordered_map<std::string, ObjectData> objects;
	std::vector<std::string> names;
	Node rootNode;
	bool isBone = false;

};