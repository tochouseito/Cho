#pragma once

#include<d3d12.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include <string>
#include <unordered_map>

/// <summary>
/// テクスチャ
/// </summary>
struct Texture {
	DirectX::TexMetadata metadata;
	uint32_t rvIndex;
};

class D3DDevice;
class D3DCommand;
class ResourceViewManager;
class TextureLoader
{
public:

	void Initialize(D3DDevice* d3dDevice, D3DCommand* d3dCommand, ResourceViewManager* rvManager);

	void Load();

	// テクスチャの取得
	Texture GetTexture(const std::string& TextureName) { return textureData[TextureName]; }

private:



private:
	D3DDevice* d3dDevice_ = nullptr;
	D3DCommand* d3dCommand_ = nullptr;
	ResourceViewManager* rvManager_ = nullptr;

	// テクスチャコンテナ
	std::unordered_map<std::string, Texture> textureData;
};

