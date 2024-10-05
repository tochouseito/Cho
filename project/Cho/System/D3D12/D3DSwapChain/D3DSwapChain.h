#pragma once

#include<dxgi1_6.h>
#include<d3d12.h>
#include<wrl.h>

class WinApp;
class D3DSwapChain
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win,IDXGIFactory7& dxgiFactory,ID3D12CommandQueue& commandQueue);

	void Present();

	ID3D12Resource* GetResource(UINT index)const { return resources_[index].Get(); }

	IDXGISwapChain4* GetSwapChain()const { return swapChain_.Get(); }

private:// メンバ変数
	/*スワップチェーン*/
	Microsoft::WRL::ComPtr < IDXGISwapChain4>swapChain_;
	DXGI_SWAP_CHAIN_DESC1 desc_{};
	Microsoft::WRL::ComPtr < ID3D12Resource>resources_[2];

	int32_t refreshRate_ = 0;
};

