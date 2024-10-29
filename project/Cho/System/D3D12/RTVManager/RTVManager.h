#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>
#include<unordered_map>

class D3DDevice;
class D3DSwapChain;
class RTVManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice,D3DSwapChain* d3dSwapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(uint32_t& index){return rtvHandles_[index]; }

	uint32_t CreateRTV(ID3D12Resource* textureResource);

	uint32_t GetNowIndex()const { return useIndex_ ; }
	static uint32_t GetMaxIndex() { return kMaxDescriptor; }

private:// メンバ関数

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	void CreateRenderTargetView();

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t& index);

private:
	/*借りインスタンス*/
	D3DDevice* d3dDevice_ = nullptr;
	D3DSwapChain* d3dSwapChain_ = nullptr;

	// 最大ディスクリプタ数
	static const uint32_t kMaxDescriptor = 8;

	// ヒープタイプ
	static const D3D12_DESCRIPTOR_HEAP_TYPE HEAP_TYPE =
		D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	/*ハンドル*/
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[kMaxDescriptor];
	// ハンドルコンテナ
	//std::unordered_map<uint32_t, D3D12_CPU_DESCRIPTOR_HANDLE> handles;



	// 次に使用するRTVインデックス。
	uint32_t useIndex_ = 0;

	
};

