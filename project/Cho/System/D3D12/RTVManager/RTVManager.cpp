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
		kMaxDescriptor,
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

uint32_t RTVManager::CreateRTV(ID3D12Resource* textureResource)
{
	uint32_t index = Allocate();

	rtvHandles_[index] = GetCPUDescriptorHandle(index);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVの設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;// 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;// 2dテクスチャとして書き込む

	d3dDevice_->GetDevice()->CreateRenderTargetView(
		textureResource,
		&rtvDesc,
		rtvHandles_[index]
	);

	return index;
}

void RTVManager::CreateRenderTargetView()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVの設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;// 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;// 2dテクスチャとして書き込む

	//// ディスクリプタの先頭取得する
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle =
	//	descriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	// RTVを２つ作るのでディスクリプタを２つ用意
	// まず１つ目を作る。１つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	uint32_t swpIndex = Allocate();
	rtvHandles_[swpIndex] = GetCPUDescriptorHandle(swpIndex);
	d3dDevice_->GetDevice()->CreateRenderTargetView(
		d3dSwapChain_->GetResource(0),
		&rtvDesc,
		rtvHandles_[swpIndex]
	);
	uint32_t swpIndex2 = Allocate();
	// ２つ目のディスクリプタハンドルを得る（自力で）
	rtvHandles_[swpIndex2] = GetCPUDescriptorHandle(swpIndex2);

	// ２つ目を作る
	d3dDevice_->GetDevice()->CreateRenderTargetView(
		d3dSwapChain_->GetResource(1),
		&rtvDesc,
		rtvHandles_[1]
	);
}

uint32_t RTVManager::Allocate()
{
	// returnする番号を一旦記録する
	int index = useIndex_;
	// 次回のため番号を1進める
	useIndex_++;
	// 上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetCPUDescriptorHandle(uint32_t& index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}
