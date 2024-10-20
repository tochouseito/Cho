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

	d3dDevice_ = d3dDevice;
}

void ResourceViewManager::SetDescriptorHeap(ID3D12GraphicsCommandList* commandList)
{
	// ディスクリプタヒープ
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };

	// コマンドリストにディスクリプタヒープをセット
	commandList->SetDescriptorHeaps(
		_countof(descriptorHeaps),
		descriptorHeaps
	);
}

uint32_t ResourceViewManager::GetNewHandle()
{
	uint32_t index = Allocate();

	ConstantHandleData& descriptorData = handles[index];

	descriptorData.CPUHandle = GetCPUDescriptorHandle(index);
	descriptorData.GPUHandle = GetGPUDescriptorHandle(index);

	return index;
}

ConstantHandleData ResourceViewManager::GetHandle(uint32_t& index)
{
	if (handles.find(index) == handles.end()) {
		assert(0);
	}
	return handles[index];
}

uint32_t ResourceViewManager::CreateCBV(const size_t& sizeInBytes)
{
	uint32_t index = CBVAllocate();

	Microsoft::WRL::ComPtr<ID3D12Resource>& resource = CBVResources[index];

	resource = CreateBufferResource(sizeInBytes);

	return index;
}

ID3D12Resource* ResourceViewManager::GetCBVResource(uint32_t& index)
{
	if (CBVResources.find(index) == CBVResources.end()) {
		return nullptr;
	}
	return CBVResources[index].Get();
}

uint32_t ResourceViewManager::CreateVBV(const size_t& sizeInBytes, uint32_t& vertices)
{
	uint32_t index = VBVAllocate();

	VBVData& vbvData = VBVResources[index];

	vbvData = CreateVBVResource(sizeInBytes, vertices);

	return index;
}

ID3D12Resource* ResourceViewManager::GetVBVResource(uint32_t& index)
{
	if (VBVResources.find(index) == VBVResources.end()) {
		return nullptr;
	}
	return VBVResources[index].resource.Get();
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

uint32_t ResourceViewManager::CBVAllocate()
{
	// returnする番号を一旦記録する
	int index = useCBVIndex_;
	// 次回のため番号を1進める
	useCBVIndex_++;
	// 上で記録した番号をreturn
	return index;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ResourceViewManager::CreateBufferResource(const size_t& sizeInBytes)
{
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;// リソースのサイズ。今回はVector4を３頂点分
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合ははこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = d3dDevice_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));
	return resource;
}

uint32_t ResourceViewManager::VBVAllocate()
{
	// returnする番号を一旦記録する
	int index = useVBVIndex_;
	// 次回のため番号を1進める
	useVBVIndex_++;
	// 上で記録した番号をreturn
	return index;
}

VBVData ResourceViewManager::CreateVBVResource(const size_t& sizeInBytes, uint32_t& vertices)
{
	VBVData vbvData;

	vbvData.resource = CreateBufferResource(sizeInBytes*static_cast<size_t>(vertices));

	// 頂点バッファビューを作成する
	// リソースの先頭のアドレスから使う
	vbvData.vbv.BufferLocation = vbvData.resource->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点のサイズ
	vbvData.vbv.SizeInBytes = static_cast<UINT>(sizeInBytes * vertices);

	// 1頂点アタリのサイズ
	vbvData.vbv.StrideInBytes = static_cast<UINT>(sizeInBytes);

	return vbvData;
}
