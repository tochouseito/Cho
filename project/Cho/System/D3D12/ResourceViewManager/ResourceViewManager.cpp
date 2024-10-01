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

ConstantHandleData ResourceViewManager::GetNewHandle()
{
	ConstantHandleData descriptorData;

	descriptorData.index = Allocate();

	descriptorData.handle.first = GetCPUDescriptorHandle(descriptorData.index);
	descriptorData.handle.second = GetGPUDescriptorHandle(descriptorData.index);

	return descriptorData;
}

uint32_t ResourceViewManager::Allocate()
{
	// returnする番号を一旦記録する
	int index = useIndex_;
	// 次回のため番号を1進める
	useIndex_++;
	// 上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE ResourceViewManager::GetCPUDescriptorHandle(uint32_t& index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE ResourceViewManager::GetGPUDescriptorHandle(uint32_t& index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}
