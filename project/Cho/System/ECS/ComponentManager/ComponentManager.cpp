#include "ComponentManager.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

// 後で消す
#include<numbers>

void ComponentManager::SetRVManager(ResourceViewManager* RVManager)
{
    RVManager_ = RVManager;
}

// EntityにTransformComponentを追加します。
// 指定されたentityにTransformComponentをtransformsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const TransformComponent& component) {
    transforms[entity] = component;
    transforms[entity].cbvIndex = RVManager_->CreateCBV(sizeof(ConstBufferDataWorldTransform));
    RVManager_->GetCBVResource(
        transforms[entity].cbvIndex)->Map(
            0, nullptr, reinterpret_cast<void**>(&transforms[entity].constData)
        );
}

// EntityにRenderComponentを追加します。
// 指定されたentityにRenderComponentをrendersマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const RenderComponent& component) {
    renders[entity] = component;
	renders[entity].visible = true;
	renders[entity].textureID = "Cho/Resources/Texture/monsterBall.png";
}

// EntityにPhysicsComponentを追加します。
// 指定されたentityにPhysicsComponentをphysicsマップに割り当てます。
void ComponentManager::AddComponent(Entity entity, const PhysicsComponent& component) {
    physics[entity] = component;
}

void ComponentManager::AddComponent(Entity entity, const MeshComponent& component)
{
    meshs[entity] = component;
    // 一旦球体生成
    std::string name = "Sphere";
    meshs[entity].names.push_back(name);
    uint32_t kSubdivision = 32;
    float kLonEvery = 2.0f * std::numbers::pi_v<float> / kSubdivision;
    float kLatEvery = std::numbers::pi_v<float> / kSubdivision;
    uint32_t kVertices = kSubdivision * kSubdivision * 6;
    MeshData& meshData = meshs[entity].meshData[name];
    meshData.vertices = kVertices;
    meshData.vbvIndex = RVManager_->CreateVBV(sizeof(VertexData), kVertices);
    RVManager_->GetVBVData(
        meshs[entity].meshData[name].vbvIndex)->resource->Map(
            0, nullptr, reinterpret_cast<void**>(&meshs[entity].meshData[name].vertexData)
        );
#pragma region
	// 緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;//4;
			float lon = lonIndex * kLonEvery; // 現在の経度
			//uint32_t startIndex = (latIndex * kSubdivision + lonIndex) * 6;//4;
			//float u = float(lonIndex) / float(kSubdivision);
			//float v = 1.0f - float(latIndex) / float(kSubdivision);
			// 頂点にデータを入力する
			// A頂点
			meshs[entity].meshData[name].vertexData[start].position.x = cos(lat) * cos(lon);
			meshs[entity].meshData[name].vertexData[start].position.y = sin(lat);
			meshs[entity].meshData[name].vertexData[start].position.z = cos(lat) * sin(lon);
			meshs[entity].meshData[name].vertexData[start].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start].texcoord = { float(lonIndex) / float(kSubdivision),1.0f - float(latIndex) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start].normal.x = meshs[entity].meshData[name].vertexData[start].position.x;
			meshs[entity].meshData[name].vertexData[start].normal.y = meshs[entity].meshData[name].vertexData[start].position.y;
			meshs[entity].meshData[name].vertexData[start].normal.z = meshs[entity].meshData[name].vertexData[start].position.z;
			// B頂点
			meshs[entity].meshData[name].vertexData[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			meshs[entity].meshData[name].vertexData[start + 1].position.y = sin(lat + kLatEvery);
			meshs[entity].meshData[name].vertexData[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			meshs[entity].meshData[name].vertexData[start + 1].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start + 1].texcoord = { float(lonIndex) / float(kSubdivision),1.0f - float(latIndex + 1) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start + 1].normal.x = meshs[entity].meshData[name].vertexData[start + 1].position.x;
			meshs[entity].meshData[name].vertexData[start + 1].normal.y = meshs[entity].meshData[name].vertexData[start + 1].position.y;
			meshs[entity].meshData[name].vertexData[start + 1].normal.z = meshs[entity].meshData[name].vertexData[start + 1].position.z;
			// C頂点
			meshs[entity].meshData[name].vertexData[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 2].position.y = sin(lat);
			meshs[entity].meshData[name].vertexData[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 2].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start + 2].texcoord = { float(lonIndex + 1) / float(kSubdivision),1.0f - float(latIndex) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start + 2].normal.x = meshs[entity].meshData[name].vertexData[start + 2].position.x;
			meshs[entity].meshData[name].vertexData[start + 2].normal.y = meshs[entity].meshData[name].vertexData[start + 2].position.y;
			meshs[entity].meshData[name].vertexData[start + 2].normal.z = meshs[entity].meshData[name].vertexData[start + 2].position.z;
			// D頂点
			meshs[entity].meshData[name].vertexData[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 3].position.y = sin(lat + kLatEvery);
			meshs[entity].meshData[name].vertexData[start + 3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 3].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start + 3].texcoord = { float(lonIndex + 1) / float(kSubdivision),1.0f - float(latIndex + 1) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start + 3].normal.x = meshs[entity].meshData[name].vertexData[start + 3].position.x;
			meshs[entity].meshData[name].vertexData[start + 3].normal.y = meshs[entity].meshData[name].vertexData[start + 3].position.y;
			meshs[entity].meshData[name].vertexData[start + 3].normal.z = meshs[entity].meshData[name].vertexData[start + 3].position.z;

			// C頂点
			meshs[entity].meshData[name].vertexData[start + 4].position.x = cos(lat) * cos(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 4].position.y = sin(lat);
			meshs[entity].meshData[name].vertexData[start + 4].position.z = cos(lat) * sin(lon + kLonEvery);
			meshs[entity].meshData[name].vertexData[start + 4].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start + 4].texcoord = { float(lonIndex + 1) / float(kSubdivision),1.0f - float(latIndex) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start + 4].normal.x = meshs[entity].meshData[name].vertexData[start + 4].position.x;
			meshs[entity].meshData[name].vertexData[start + 4].normal.y = meshs[entity].meshData[name].vertexData[start + 4].position.y;
			meshs[entity].meshData[name].vertexData[start + 4].normal.z = meshs[entity].meshData[name].vertexData[start + 4].position.z;
			// B頂点
			meshs[entity].meshData[name].vertexData[start + 5].position.x = cos(lat + kLatEvery) * cos(lon);
			meshs[entity].meshData[name].vertexData[start + 5].position.y = sin(lat + kLatEvery);
			meshs[entity].meshData[name].vertexData[start + 5].position.z = cos(lat + kLatEvery) * sin(lon);
			meshs[entity].meshData[name].vertexData[start + 5].position.w = 1.0f;
			meshs[entity].meshData[name].vertexData[start + 5].texcoord = { float(lonIndex) / float(kSubdivision),1.0f - float(latIndex + 1) / float(kSubdivision) };
			meshs[entity].meshData[name].vertexData[start + 5].normal.x = meshs[entity].meshData[name].vertexData[start + 5].position.x;
			meshs[entity].meshData[name].vertexData[start + 5].normal.y = meshs[entity].meshData[name].vertexData[start + 5].position.y;
			meshs[entity].meshData[name].vertexData[start + 5].normal.z = meshs[entity].meshData[name].vertexData[start + 5].position.z;

		}
	}
#pragma endregion
}

void ComponentManager::AddComponent(Entity entity, const CameraComponent& component)
{
	cameras[entity] = component;
	cameras[entity].cbvIndex = RVManager_->CreateCBV(sizeof(ConstBufferDataViewProjection));
	RVManager_->GetCBVResource(
		cameras[entity].cbvIndex)->Map(
			0, nullptr, reinterpret_cast<void**>(&cameras[entity].constData)
		);
}

// Entityに関連するすべてのコンポーネントを削除します。
// 指定されたentityに対して、TransformComponent、RenderComponent、PhysicsComponentをそれぞれのマップから削除します。
void ComponentManager::RemoveComponent(Entity entity) {
    transforms.erase(entity);
    renders.erase(entity);
    physics.erase(entity);
    meshs.erase(entity);
	cameras.erase(entity);
}
