#pragma once

#include<string>
#include<cstdint>

class ResourceViewManager;
class MeshLoader
{
public:
	void Initialize(ResourceViewManager* rvManager);

	uint32_t LoadMesh(const std::string& name, uint32_t& vertices, uint32_t& indices);

	void Map(uint32_t& index,const std::string& name, const std::string& modelName);
private:
	ResourceViewManager* rvManager_ = nullptr;
};

