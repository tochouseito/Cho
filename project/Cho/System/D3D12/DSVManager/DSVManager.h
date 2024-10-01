#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>

class WinApp;
class D3DDevice;
class DSVManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win, D3DDevice* d3dDevice);

	ID3D12DescriptorHeap* GetDescriptorHeap()const { return descriptorHeap_.Get(); }

private:// メンバ関数

	/// <summary>
	/// 深度リソースの作成
	/// </summary>
	void CreateDepthStencil();

	/// <summary>
	/// 深度リソースの作成
	/// </summary>
	Microsoft::WRL::ComPtr < ID3D12Resource	> CreateDepthStencilTextureResource(
		int32_t width, int32_t height
	);

private:
	/*借りインスタンス*/
	WinApp* win_ = nullptr;
	D3DDevice* d3dDevice_ = nullptr;

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	// 深度リソース
	Microsoft::WRL::ComPtr < ID3D12Resource>resource_;
};

