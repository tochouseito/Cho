#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>

class D3DDevice;
class ResourceView
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice);

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

