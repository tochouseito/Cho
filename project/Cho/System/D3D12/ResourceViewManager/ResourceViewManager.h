#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>
#include<unordered_map>
#include"ECS/EntityManager/EntityManager.h"

// ディスクリプタハンドル定数データ
struct ConstantHandleData {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
};

class D3DDevice;
class ResourceViewManager
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice);

	/// <summary>
	/// ディスクリプタヒープをセット
	/// </summary>
	/// <param name="commandList"></param>
	void SetDescriptorHeap(ID3D12GraphicsCommandList* commandList);

	ID3D12DescriptorHeap* GetDescriptorHeap()const { return descriptorHeap_.Get(); }

	/// <summary>
	/// 新しいハンドルを入手する
	/// </summary>
	/// <returns></returns>
	uint32_t GetNewHandle();

	// ハンドルを取得
	ConstantHandleData GetHandle(uint32_t& index);

	uint32_t CreateCBV(const size_t& sizeInBytes);

	ID3D12Resource* GetCBVResource(uint32_t& index);

private:

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t& index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t& index);

	uint32_t CBVAllocate();

	// CBVリソース作成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource(const size_t& sizeInBytes);

private:// メンバ変数

	D3DDevice* d3dDevice_ = nullptr;

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// 次に使用するSRVインデックス。先頭はImGuiで使うので1から
	uint32_t useIndex_ = 1;

	// 最大ディスクリプタ数
	static const uint32_t kMaxDescriptor = 1024;

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// ハンドルコンテナ
	std::unordered_map<uint32_t, ConstantHandleData> handles;

	// 次に使用するCBVインデックス
	uint32_t useCBVIndex_ = 0;

	// CBVコンテナ
	std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID3D12Resource>> CBVResources;
};

