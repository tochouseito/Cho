#include "PrecompiledHeader.h"
#include "MeshLoader.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void MeshLoader::Initialize(ResourceViewManager* rvManager)
{
	rvManager_ = rvManager;
}

uint32_t MeshLoader::LoadMesh(const std::string& name,uint32_t& vertices, uint32_t& indices)
{
	return rvManager_->CreateMeshResource(name, vertices, indices);
}

void MeshLoader::Map(uint32_t& index, const std::string& name, const std::string& modelName)
{
	rvManager_->MeshMap(index, name,modelName);
}
