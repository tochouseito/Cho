#pragma once

#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<cstdint>
#include<unordered_map>
#include<array>
#include<memory>
#include"ECS/EntityManager/EntityManager.h"
#include"Color.h"
#include"ConstantData/ModelData.h"
#include"ECS/ComponentManager/Components/Components.h"

// ディスクリプタハンドル定数データ
struct ConstantHandleData {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
};
struct VBVData {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_VERTEX_BUFFER_VIEW vbv{};
};
struct IBVData {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_INDEX_BUFFER_VIEW ibv{};
};
struct MeshView {
	VBVData vbvData;
	IBVData ibvData;
};
class D3DDevice;
class D3DCommand;
class ResourceViewManager
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice, D3DCommand* d3dCommand);

	/// <summary>
	/// ディスクリプタヒープをセット
	/// </summary>
	/// <param name="commandList"></param>
	void SetDescriptorHeap(ID3D12GraphicsCommandList* commandList);

	ID3D12DescriptorHeap* GetDescriptorHeap()const { return descriptorHeap_.Get(); }

	uint32_t GetNowIndex()const { return useIndex_ + useCBVIndex_; }
	static uint32_t GetMaxIndex() { return kMaxDescriptor; }

	/// <summary>
	/// 新しいハンドルを入手する
	/// </summary>
	/// <returns></returns>
	uint32_t GetNewHandle();

	// ハンドルを取得
	ConstantHandleData GetHandle(const uint32_t& index);

	uint32_t CreateCBV(const size_t& sizeInBytes);

	ID3D12Resource* GetCBVResource(const uint32_t& index);

	uint32_t CreateMeshView(const uint32_t& vertices, const uint32_t& indices, const size_t& sizeInBytes,ID3D12Resource* pResource=nullptr);

	MeshView* GetMeshViewData(const uint32_t& index);

	void CreateTextureResource(const uint32_t& index,const DirectX::TexMetadata& metadata);

	void UploadTextureDataEx(const uint32_t& index,const DirectX::ScratchImage& mipImages);

	void CreateUAVResource(const uint32_t& index, const size_t& sizeInBytes);

	void CreateUAVforStructuredBuffer(const uint32_t& index,const UINT& numElements, const UINT& structuredByteStride);

	void CreateSRVResource(const uint32_t& index, const size_t& sizeInBytes);

	void CreateSRVForTexture2D(const uint32_t& index, const DXGI_FORMAT& Format, const UINT& MipLevels);

	// SRV生成(StructuredBuffer用)
	void CreateSRVforStructuredBuffer(const uint32_t& index, const UINT& numElements, const UINT& structuredByteStride);

	void CreateRenderTextureResource(
		const uint32_t& index,
		const uint32_t& width,
		const uint32_t& height,
		DXGI_FORMAT format,
		const Color& clearColor
	);

	// デフォルトメッシュパターン生成
	void CreateMeshPattern();

	// デフォルトメッシュ用の専用関数
	void CreateMeshViewDMP(const uint32_t& index, const uint32_t& vertices, const uint32_t& indices);

	Meshs* GetMesh(const uint32_t& index)const { return meshContainer[index].get(); }

	uint32_t CreateMeshResource(const std::string& name, const uint32_t& vertices, const uint32_t& indices, const size_t& sizeInBytes,ID3D12Resource* pResource=nullptr);

	void ModelMeshMap(const uint32_t& index, const std::string& name);

	void MeshDataCopy(const uint32_t& index, const std::string& name, const std::string& modelName);

	void AddModel(const std::string& name);

	ModelData* GetModelData(const std::string& name);

	const std::vector<std::unique_ptr<Meshs>>& GetMeshs() { return meshContainer; }

	const std::unordered_map<std::string, std::unique_ptr<ModelData>>& GetModels() { return modelContainer; }

	const std::unordered_map<uint32_t, std::unique_ptr<SpriteMeshData>>& GetSprites() { return spriteContainer; }

	SpriteMeshData* GetSpriteData(const uint32_t& index);

	uint32_t CreateSpriteData();
private:

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t& index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t& index);

	uint32_t CBVAllocate();

	// CBVリソース作成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource(const size_t& sizeInBytes);

	uint32_t MeshViewAllocate();

	// VBVリソース作成
	MeshView CreateMeshViewResource(const uint32_t& vertices, const uint32_t& indices, const size_t& sizeInBytes, ID3D12Resource* pResource=nullptr);

	uint32_t SpriteAllocate();

private:// メンバ変数

	D3DDevice* d3dDevice_ = nullptr;
	D3DCommand* d3dCommand_ = nullptr;

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// 次に使用するSRV,UAVインデックス。先頭はImGuiで使うので1から
	uint32_t useIndex_ = 1;

	// 最大ディスクリプタ数
	static const uint32_t kMaxDescriptor = 1024;

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// SRV,UAVコンテナ
	std::unordered_map<uint32_t, ConstantHandleData> handles;

	// 次に使用するCBVインデックス
	uint32_t useCBVIndex_ = 0;

	// CBVコンテナ
	std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID3D12Resource>> CBVResources;

	// 次に使用するVBVインデックス
	uint32_t useMeshViewIndex_ = 0;

	// MeshViewコンテナ
	std::unordered_map<uint32_t, MeshView> meshViews;

	// メッシュコンテナ
	std::vector<std::unique_ptr<Meshs>> meshContainer;

	// アップロードリソース
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> uploadResources;

	// Modelコンテナ
	std::unordered_map<std::string, std::unique_ptr<ModelData>> modelContainer;


	// 次に使用するスプライトインデックス
	uint32_t useSpriteIndex_ = 0;
	// スプライトコンテナ
	std::unordered_map<uint32_t, std::unique_ptr<SpriteMeshData>> spriteContainer;

};

