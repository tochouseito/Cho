#pragma once

#include<dxgi1_6.h>
#include<d3d12.h>
#include<wrl.h>

class D3DSwapChain
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:// メンバ変数
	/*スワップチェーン*/
	Microsoft::WRL::ComPtr < IDXGISwapChain4>swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr < ID3D12Resource>swapChainResources_[2];
};

