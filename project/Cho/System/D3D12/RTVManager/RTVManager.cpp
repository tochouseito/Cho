#include "PrecompiledHeader.h"
#include "RTVManager.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"

void RTVManager::Initialize(D3DDevice* d3dDevice, D3DSwapChain* d3dSwapChain)
{
	d3dDevice_ = d3dDevice;
	d3dSwapChain_ = d3dSwapChain;

	// デスクリプタヒープの生成
	descriptorHeap_ = d3dDevice_->CreateDescriptorHeap(
		HEAP_TYPE,
		3,
		false
	);
	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ =
		d3dDevice_->GetDevice()->GetDescriptorHandleIncrementSize(
			HEAP_TYPE
		);

	// RTVの作成
	CreateRenderTargetView();
}

void RTVManager::CreateRenderTargetView()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVの設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;// 2dテクスチャとして書き込む

	// ディスクリプタの先頭取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle =
		descriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	// RTVを２つ作るのでディスクリプタを２つ用意
	// まず１つ目を作る。１つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle;
	d3dDevice_->GetDevice()->CreateRenderTargetView(
		d3dSwapChain_->GetResource(0),
		&rtvDesc,
		rtvHandles_[0]
	);

	// ２つ目のディスクリプタハンドルを得る（自力で）
	rtvHandles_[1].ptr = 
		rtvHandles_[0].ptr + d3dDevice_->GetDevice()->GetDescriptorHandleIncrementSize(
			HEAP_TYPE
		);

	// ２つ目を作る
	d3dDevice_->GetDevice()->CreateRenderTargetView(
		d3dSwapChain_->GetResource(1),
		&rtvDesc,
		rtvHandles_[1]
	);
}
