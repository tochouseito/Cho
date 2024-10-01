#pragma once

#include<d3d12.h>
#include<wrl.h>

class D3DDevice;
class D3DSwapChain;
class RTVManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice,D3DSwapChain* d3dSwapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(uint32_t& index)const {return rtvHandles_[index]; }

private:// メンバ関数

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	void CreateRenderTargetView();

private:
	/*借りインスタンス*/
	D3DDevice* d3dDevice_ = nullptr;
	D3DSwapChain* d3dSwapChain_ = nullptr;

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	/*ハンドル*/
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[3];

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
};

