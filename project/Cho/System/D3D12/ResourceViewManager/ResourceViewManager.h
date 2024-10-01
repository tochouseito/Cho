#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>

// ディスクリプタハンドル定数データ
struct ConstantHandleData {
	uint32_t index = 0;
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
	ConstantHandleData GetNewHandle();

private:

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t& index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t& index);

private:// メンバ変数

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// 次に使用するSRVインデックス。先頭はImGuiで使うので1から
	uint32_t useIndex_ = 1;

	// 最大ディスクリプタ数
	static const uint32_t kMaxDescriptor = 512;

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

};

