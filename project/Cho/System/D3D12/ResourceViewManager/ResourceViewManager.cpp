#include "PrecompiledHeader.h"
#include "ResourceViewManager.h"
#include"D3D12/D3DDevice/D3DDevice.h"

void ResourceViewManager::Initialize(D3DDevice* d3dDevice)
{
	// デスクリプタヒープの生成
	descriptorHeap_ = d3dDevice->CreateDescriptorHeap(
		HEAP_TYPE,
		kMaxDescriptor,
		true
	);
	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ = d3dDevice->GetDevice()->GetDescriptorHandleIncrementSize(
		HEAP_TYPE
	);
}

void ResourceViewManager::SetDescriptorHeap(ID3D12GraphicsCommandList& commandList)
{
	// ディスクリプタヒープ
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };

	// コマンドリストにディスクリプタヒープをセット
	commandList.SetDescriptorHeaps(
		_countof(descriptorHeaps),
		descriptorHeaps
	);
}