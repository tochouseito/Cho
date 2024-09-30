#include "PrecompiledHeader.h"
#include "D3DSwapChain.h"
#include"WinApp/WinApp.h"

void D3DSwapChain::Initialize(WinApp* win, IDXGIFactory7& dxgiFactory, ID3D12CommandQueue& commandQueue)
{
	HRESULT hr;

	// スワップチェーンを生成する
	swapChainDesc_.Width = win->kClientWidth;                     // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = win->kClientHeight;                   // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;          // 色の形式
	swapChainDesc_.SampleDesc.Count = 1;                         // マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// 描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;                              // ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   // モニタにうつしたら、中身を破棄

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory.CreateSwapChainForHwnd(
		&commandQueue, win->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf())
	);

	assert(SUCCEEDED(hr));

	// SwapChainからResourceを引っ張ってくる
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
}
