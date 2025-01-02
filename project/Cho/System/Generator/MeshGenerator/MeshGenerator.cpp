#include "PrecompiledHeader.h"
#include "MeshGenerator.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"

Meshs MeshGenerator::GeneratorMeshs(MeshPattern pattern, ResourceViewManager* rvManager)
{
	Meshs meshs;
	switch (pattern)
	{
	case MeshPattern::Cube:
	{
		// 立方体生成
		std::string name = "Cube";
		meshs.names.push_back(name);
		MeshData& meshData = meshs.meshData[name];
		meshData.meshViewIndex = static_cast<uint32_t>(pattern);
		meshData.size = CreateCube(meshData, rvManager);
	}
		break;
	case MeshPattern::Plane:
	{
		// 平面生成
		std::string name = "Plane";
		meshs.names.push_back(name);
		MeshData& meshData = meshs.meshData[name];
		meshData.meshViewIndex = static_cast<uint32_t>(pattern);
		meshData.size = CreatePlane(meshData, rvManager);
	}
		break;
	case MeshPattern::Sphere:
	{
		// 球体生成
		std::string name = "Sphere";
		meshs.names.push_back(name);
		MeshData& meshData = meshs.meshData[name];
		meshData.meshViewIndex = static_cast<uint32_t>(pattern);
		meshData.size = CreateSphere(meshData, rvManager);
	}
		break;
	case MeshPattern::CountPattern:
		// ここを通るはずがない
		assert(0);
		break;
	default:
		break;
	}
	return meshs;
}

VertexSize MeshGenerator::CreateSphere(MeshData& data, ResourceViewManager* rvManager)
{
	VertexSize result;

	uint32_t kSubdivision = 32;
	float kLonEvery = 2.0f * std::numbers::pi_v<float> / kSubdivision;
	float kLatEvery = std::numbers::pi_v<float> / kSubdivision;
	uint32_t kVertices = kSubdivision * kSubdivision * 4;
	uint32_t kIndices = kSubdivision * kSubdivision * 6;

	result.vertices = kVertices;
	result.indices = kIndices;

	rvManager->CreateMeshViewDMP(data.meshViewIndex, kVertices,kIndices);
	rvManager->GetMeshViewData(data.meshViewIndex)->vbvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.vertexData)
	);
	rvManager->GetMeshViewData(data.meshViewIndex)->ibvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.indexData)
	);
#pragma region
	// 緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 4;//4;
			float lon = lonIndex * kLonEvery; // 現在の経度
			//uint32_t startIndex = (latIndex * kSubdivision + lonIndex) * 6;//4;
			//float u = float(lonIndex) / float(kSubdivision);
			//float v = 1.0f - float(latIndex) / float(kSubdivision);
			// 頂点にデータを入力する
			// A頂点
			data.vertexData[start].position.x = cos(lat) * cos(lon);
			data.vertexData[start].position.y = sin(lat);
			data.vertexData[start].position.z = cos(lat) * sin(lon);
			data.vertexData[start].position.w = 1.0f;
			data.vertexData[start].texcoord = { float(lonIndex) / float(kSubdivision),1.0f - float(latIndex) / float(kSubdivision) };
			data.vertexData[start].normal.x = data.vertexData[start].position.x;
			data.vertexData[start].normal.y = data.vertexData[start].position.y;
			data.vertexData[start].normal.z = data.vertexData[start].position.z;
			// B頂点
			data.vertexData[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			data.vertexData[start + 1].position.y = sin(lat + kLatEvery);
			data.vertexData[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			data.vertexData[start + 1].position.w = 1.0f;
			data.vertexData[start + 1].texcoord = { float(lonIndex) / float(kSubdivision),1.0f - float(latIndex + 1) / float(kSubdivision) };
			data.vertexData[start + 1].normal.x = data.vertexData[start + 1].position.x;
			data.vertexData[start + 1].normal.y = data.vertexData[start + 1].position.y;
			data.vertexData[start + 1].normal.z = data.vertexData[start + 1].position.z;
			// C頂点
			data.vertexData[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			data.vertexData[start + 2].position.y = sin(lat);
			data.vertexData[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			data.vertexData[start + 2].position.w = 1.0f;
			data.vertexData[start + 2].texcoord = { float(lonIndex + 1) / float(kSubdivision),1.0f - float(latIndex) / float(kSubdivision) };
			data.vertexData[start + 2].normal.x = data.vertexData[start + 2].position.x;
			data.vertexData[start + 2].normal.y = data.vertexData[start + 2].position.y;
			data.vertexData[start + 2].normal.z = data.vertexData[start + 2].position.z;
			// D頂点
			data.vertexData[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			data.vertexData[start + 3].position.y = sin(lat + kLatEvery);
			data.vertexData[start + 3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			data.vertexData[start + 3].position.w = 1.0f;
			data.vertexData[start + 3].texcoord = { float(lonIndex + 1) / float(kSubdivision),1.0f - float(latIndex + 1) / float(kSubdivision) };
			data.vertexData[start + 3].normal.x = data.vertexData[start + 3].position.x;
			data.vertexData[start + 3].normal.y = data.vertexData[start + 3].position.y;
			data.vertexData[start + 3].normal.z = data.vertexData[start + 3].position.z;

		}
	}
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			uint32_t startIndex = (latIndex * kSubdivision + lonIndex) * 4;
			data.indexData[start] = 0 + startIndex;
			data.indexData[start + 1] = 1 + startIndex;
			data.indexData[start + 2] = 2 + startIndex;
			data.indexData[start + 3] = 1 + startIndex;
			data.indexData[start + 4] = 3 + startIndex;
			data.indexData[start + 5] = 2 + startIndex;
		}
	}
#pragma endregion
	return result;
}

VertexSize MeshGenerator::CreateCube(MeshData& data, ResourceViewManager* rvManager)
{
	VertexSize result;

	uint32_t boxVertex = 24;
	uint32_t indices = 36;

	result.vertices = boxVertex;
	result.indices = indices;

	rvManager->CreateMeshViewDMP(data.meshViewIndex, boxVertex,indices);
	rvManager->GetMeshViewData(data.meshViewIndex)->vbvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.vertexData)
	);
	rvManager->GetMeshViewData(data.meshViewIndex)->ibvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.indexData)
	);
#pragma region
	// 右面
	data.vertexData[0] = { {1.0f,  1.0f,  1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }; // 右上
	data.vertexData[1] = { {1.0f,  1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }; // 左上
	data.vertexData[2] = { {1.0f, -1.0f,  1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} }; // 右下
	data.vertexData[3] = { {1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} }; // 左下

	// 左面
	data.vertexData[4] = { {-1.0f,  1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} }; // 左上
	data.vertexData[5] = { {-1.0f,  1.0f,  1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} }; // 右上
	data.vertexData[6] = { {-1.0f, -1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} }; // 左下
	data.vertexData[7] = { {-1.0f, -1.0f,  1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} }; // 右下

	// 前面
	data.vertexData[8] = { {-1.0f,  1.0f,  1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }; // 左上
	data.vertexData[9] = { { 1.0f,  1.0f,  1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }; // 右上
	data.vertexData[10] = { {-1.0f, -1.0f,  1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }; // 左下
	data.vertexData[11] = { { 1.0f, -1.0f,  1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }; // 右下

	// 後面
	data.vertexData[12] = { { 1.0f,  1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} }; // 右上
	data.vertexData[13] = { {-1.0f,  1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} }; // 左上
	data.vertexData[14] = { { 1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} }; // 右下
	data.vertexData[15] = { {-1.0f, -1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} }; // 左下

	// 上面
	data.vertexData[16] = { {-1.0f,  1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }; // 左奥
	data.vertexData[17] = { { 1.0f,  1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }; // 右奥
	data.vertexData[18] = { {-1.0f,  1.0f,  1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} }; // 左前
	data.vertexData[19] = { { 1.0f,  1.0f,  1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} }; // 右前

	// 下面
	data.vertexData[20] = { {-1.0f, -1.0f,  1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} }; // 左前
	data.vertexData[21] = { { 1.0f, -1.0f,  1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} }; // 右前
	data.vertexData[22] = { {-1.0f, -1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }; // 左奥
	data.vertexData[23] = { { 1.0f, -1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }; // 右奥

	// 右面インデックス
	data.indexData[0] = 0; data.indexData[1] = 2; data.indexData[2] = 1;
	data.indexData[3] = 2; data.indexData[4] = 3; data.indexData[5] = 1;

	// 左面インデックス
	data.indexData[6] = 4; data.indexData[7] = 6; data.indexData[8] = 5;
	data.indexData[9] = 6; data.indexData[10] = 7; data.indexData[11] = 5;

	// 前面インデックス
	data.indexData[12] = 8; data.indexData[13] = 10; data.indexData[14] = 9;
	data.indexData[15] = 10; data.indexData[16] = 11; data.indexData[17] = 9;

	// 後面インデックス
	data.indexData[18] = 12; data.indexData[19] = 14; data.indexData[20] = 13;
	data.indexData[21] = 14; data.indexData[22] = 15; data.indexData[23] = 13;

	// 上面インデックス
	data.indexData[24] = 16; data.indexData[25] = 18; data.indexData[26] = 17;
	data.indexData[27] = 18; data.indexData[28] = 19; data.indexData[29] = 17;

	// 下面インデックス
	data.indexData[30] = 20; data.indexData[31] = 22; data.indexData[32] = 21;
	data.indexData[33] = 22; data.indexData[34] = 23; data.indexData[35] = 21;
#pragma endregion
	return result;
}

VertexSize MeshGenerator::CreatePlane(MeshData& data, ResourceViewManager* rvManager)
{
	VertexSize result;

	uint32_t vertices = 4;
	uint32_t indices = 6;

	result.vertices = vertices;
	result.indices = indices;

	rvManager->CreateMeshViewDMP(data.meshViewIndex, vertices,indices);
	rvManager->GetMeshViewData(data.meshViewIndex)->vbvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.vertexData)
	);

	rvManager->GetMeshViewData(data.meshViewIndex)->ibvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.indexData)
	);

#pragma region
	// 頂点データ（重複なし）
	data.vertexData[0] = { { 1.0f,  1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };  // 右上
	data.vertexData[1] = { {-1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };  // 左上
	data.vertexData[2] = { { 1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } };  // 右下
	data.vertexData[3] = { {-1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } };  // 左下

	data.indexData[0] = 0;
	data.indexData[1] = 1;
	data.indexData[2] = 2;
	data.indexData[3] = 1;
	data.indexData[4] = 3;
	data.indexData[5] = 2;
#pragma endregion
	return result;
}

void MeshGenerator::CreateSprite(SpriteMeshData& data, ResourceViewManager* rvManager)
{
	VertexSize result;

	uint32_t vertices = 4;
	uint32_t indices = 6;

	data.size.vertices = vertices;
	data.size.indices = indices;

	data.meshViewIndex=rvManager->CreateMeshView(data.size.vertices, data.size.indices);

	rvManager->GetMeshViewData(data.meshViewIndex)->vbvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.vertexData)
	);

	rvManager->GetMeshViewData(data.meshViewIndex)->ibvData.resource->Map(
		0, nullptr, reinterpret_cast<void**>(&data.indexData)
	);

	data.vertexData[0] = { {0.0f,360.0f},{0.0f,1.0f} };  // 左下
	data.vertexData[1] = { {0.0f,0.0f},{0.0f,0.0f} };	 // 左上
	data.vertexData[2] = { {640.0f,360.0f},{1.0f,1.0f} };// 右下
	data.vertexData[3] = { {640.0f,0.0f},{1.0f,0.0f} };	 // 右上
	
	data.indexData[0] = 0;
	data.indexData[1] = 1;
	data.indexData[2] = 2;
	data.indexData[3] = 1;
	data.indexData[4] = 3;
	data.indexData[5] = 2;
}
