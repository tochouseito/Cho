#include"PrecompiledHeader.h"
#include "ResourceView.h"
#include"D3D12/D3DDevice/D3DDevice.h"

void ResourceView::Initialize(D3DDevice* d3dDevice)
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
