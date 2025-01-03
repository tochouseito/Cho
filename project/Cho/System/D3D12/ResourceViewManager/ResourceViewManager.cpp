#include "PrecompiledHeader.h"
#include "ResourceViewManager.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/D3DCommand/D3DCommand.h"

void ResourceViewManager::Initialize(D3DDevice* d3dDevice, D3DCommand* d3dCommand)
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
	d3dCommand_ = d3dCommand;
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

ConstantHandleData ResourceViewManager::GetHandle(uint32_t index)
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

ID3D12Resource* ResourceViewManager::GetCBVResource(const uint32_t& index)
{
	if (CBVResources.find(index) == CBVResources.end()) {
		return nullptr;
	}
	return CBVResources[index].Get();
}

uint32_t ResourceViewManager::CreateMeshView(const uint32_t& vertices, const uint32_t& indices, const size_t& sizeInBytes)
{
	uint32_t index = MeshViewAllocate();

	MeshView& meshView = meshViews[index];

	//size_t sizeInBytes = sizeof(VertexData);

	meshView = CreateMeshViewResource(vertices,indices,sizeInBytes);

	return index;
}

MeshView* ResourceViewManager::GetMeshViewData(const uint32_t& index)
{
	if (meshViews.find(index) == meshViews.end()) {
		return nullptr;
	}
	return &meshViews[index];
}

void ResourceViewManager::CreateTextureResource(const uint32_t& index, const DirectX::TexMetadata& metadata)
{
	// 1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);// Textureの幅
	resourceDesc.Height = UINT(metadata.height);// Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);// mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);// 奥行き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;// サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);// Textureの次元数。普段使っているのは2次元
	// 2.利用するHeapの設定 非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;// 細かい設定を行う
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;// WriteBackポリシーでCPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;// プロセッサの近くに配置
	//heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;// 細かい設定を行う
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;// WriteBackポリシーでCPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;// プロセッサの近くに配置
	// 3.Resourceを生成する
	Microsoft::WRL::ComPtr < ID3D12Resource> resource = nullptr;
	HRESULT hr;hr = d3dDevice_->GetDevice()->CreateCommittedResource(
		&heapProperties,// Heapの設定
		D3D12_HEAP_FLAG_NONE,// Heapの特殊な設定。特になし
		&resourceDesc,// Resourceの設定
		//D3D12_RESOURCE_STATE_GENERIC_READ,// 初回のResourceState。Textureは基本読むだけ
		D3D12_RESOURCE_STATE_COPY_DEST,// 初回のResourceState。Textureは基本読むだけ
		nullptr,// Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));// 作成するResourceポインタのポインタ
	assert(SUCCEEDED(hr));

	handles[index].resource = resource;
}

void ResourceViewManager::UploadTextureDataEx(const uint32_t& index, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(d3dDevice_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(handles[index].resource.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr < ID3D12Resource> intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(d3dCommand_->GetCommandList(), handles[index].resource.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	// Textureへの転送後は利用できるよう,D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = handles[index].resource.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dCommand_->GetCommandList()->ResourceBarrier(1, &barrier);
	uploadResources.push_back(intermediateResource);
}

void ResourceViewManager::CreateSRVForTexture2D(const uint32_t& index, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	// metadataを基にSRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;// 2Dのテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);

	d3dDevice_->GetDevice()->CreateShaderResourceView(
		handles[index].resource.Get(), 
		&srvDesc,
		handles[index].CPUHandle
	);
}

void ResourceViewManager::CreateRenderTextureResource(const uint32_t& index, const uint32_t& width, const uint32_t& height, DXGI_FORMAT format, const Color& clearColor)
{
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;// Textureの幅
	resourceDesc.Height = height;// Textureの高さ
	resourceDesc.MipLevels = 1;//mipmapの数
	resourceDesc.DepthOrArraySize = 1;// 奥行き or 配列Textureの配列数
	resourceDesc.Format = format;// DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;// サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;// 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;// RenderTargetとして利用可能にする

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;// VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.r;
	clearValue.Color[1] = clearColor.g;
	clearValue.Color[2] = clearColor.b;
	clearValue.Color[3] = clearColor.a;

	// Resourceの生成
	Microsoft::WRL::ComPtr < ID3D12Resource> resource = nullptr;
	HRESULT hr;hr = d3dDevice_->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,// Clear最適値。ClearRenderTargetをこの色でClearするようにする。最適化されているので高速である
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));
	handles[index].resource = resource;

	CreateSRVForTexture2D(index, format, 1);
}


SpriteMeshData* ResourceViewManager::GetSpriteData(const uint32_t& index)
{
	if (!spriteContainer.contains(index)) {
		return nullptr;
	}
	return spriteContainer[index].get();
}

uint32_t ResourceViewManager::CreateSpriteData()
{
	uint32_t index = SpriteAllocate();
	spriteContainer[index] = std::make_unique<SpriteMeshData>();
	SpriteMeshData* data = spriteContainer[index].get();

	MeshGenerator::CreateSprite(*data, this);

	return index;
}

uint32_t ResourceViewManager::Allocate()
{
	if (useIndex_+useCBVIndex_ >= kMaxDescriptor) {
		assert(0);
	}
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
	if (useIndex_ + useCBVIndex_ >= kMaxDescriptor) {
		assert(0);
	}
	// returnする番号を一旦記録する
	int index = useCBVIndex_;
	// 次回のため番号を1進める
	useCBVIndex_++;
	// 上で記録した番号をreturn
	return index;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ResourceViewManager::CreateBufferResource(const size_t& sizeInBytes)
{
	// リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeapを使う
	// リソースの設定
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
	// 実際にリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr;hr = d3dDevice_->GetDevice()->CreateCommittedResource(
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

uint32_t ResourceViewManager::MeshViewAllocate()
{
	// returnする番号を一旦記録する
	int index = useMeshViewIndex_;
	// 次回のため番号を1進める
	useMeshViewIndex_++;
	// 上で記録した番号をreturn
	return index;
}

MeshView ResourceViewManager::CreateMeshViewResource(const uint32_t& vertices, const uint32_t& indices,const size_t& sizeInBytes)
{
	MeshView meshView;

	//size_t sizeInBytes = sizeof(VertexData);

	meshView.vbvData.resource = CreateBufferResource(sizeInBytes * static_cast<size_t>(vertices));

	// 頂点バッファビューを作成する
	// リソースの先頭のアドレスから使う
	meshView.vbvData.vbv.BufferLocation = meshView.vbvData.resource->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点のサイズ
	meshView.vbvData.vbv.SizeInBytes = static_cast<UINT>(sizeInBytes * vertices);

	// 1頂点アタリのサイズ
	meshView.vbvData.vbv.StrideInBytes = static_cast<UINT>(sizeInBytes);

	// インデックスバッファービューを作成する
	meshView.ibvData.resource = CreateBufferResource(sizeof(uint32_t) * static_cast<size_t>(indices));
	meshView.ibvData.ibv.BufferLocation = meshView.ibvData.resource->GetGPUVirtualAddress();
	meshView.ibvData.ibv.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indices);
	meshView.ibvData.ibv.Format = DXGI_FORMAT_R32_UINT;

	return meshView;
}

uint32_t ResourceViewManager::SpriteAllocate()
{
	uint32_t index = useSpriteIndex_;
	useSpriteIndex_++;
	return index;
}

void ResourceViewManager::CreateMeshPattern()
{
	for (uint32_t index = 0; index < static_cast<uint32_t>(MeshPattern::CountPattern); index++) {
		if (MeshViewAllocate() != index) {
			assert(0);
		}

		// indexをMeshPattern型にキャストしてGeneratorMeshsに渡す
		meshContainer.push_back(
			std::make_unique<Meshs>(MeshGenerator::GeneratorMeshs(static_cast<MeshPattern>(index), this))
		);
	}
}

void ResourceViewManager::CreateMeshViewDMP(const uint32_t& index, const  uint32_t& vertices, const uint32_t& indices)
{
	//VBVData& vbvData = VBVResources[index];
	MeshView& meshView = meshViews[index];

	//size_t sizeInBytes = sizeof(VertexData);

	meshView = CreateMeshViewResource(vertices,indices,sizeof(VertexData));
}

uint32_t ResourceViewManager::CreateMeshResource(const std::string& name, const uint32_t& vertices, const uint32_t& indices)
{
	uint32_t index = CreateMeshView(vertices, indices,sizeof(VertexData));

	std::unique_ptr<Meshs> mesh = std::make_unique<Meshs>();

	mesh->names.push_back(name);
	MeshData& meshData = mesh->meshData[name];
	meshData.meshViewIndex = index;
	meshData.size.vertices = vertices;
	meshData.size.indices = indices;

	meshContainer.push_back(std::move(mesh));

	return index;
}

void ResourceViewManager::ModelMeshMap(const uint32_t& index, const std::string& name, const std::string& modelName)
{
	Meshs* meshs = meshContainer[index].get();
	ModelData* modelData = modelContainer[modelName].get();

	meshViews[index].vbvData.resource->Map(
		0, nullptr, 
		reinterpret_cast<void**>(&meshs->meshData[name].vertexData)
	);

	meshViews[index].ibvData.resource->Map(
		0, nullptr,
		reinterpret_cast<void**>(&meshs->meshData[name].indexData)
	);

	// 頂点データをリソースにコピー
	std::memcpy(meshs->meshData[name].vertexData,
		modelData->objects[name].vertices.data(),
		sizeof(VertexData) * modelData->objects[name].vertices.size()
	);
	// インデックスデータをリソースにコピー
	std::memcpy(meshs->meshData[name].indexData,
		modelData->objects[name].indices.data(),
		sizeof(uint32_t) * modelData->objects[name].indices.size()
	);
}

void ResourceViewManager::AddModel(const std::string& name)
{
	modelContainer[name] = std::make_unique<ModelData>();
}

ModelData* ResourceViewManager::GetModelData(const std::string& name)
{
	if (!modelContainer.contains(name)) {
		assert(0);
	}
	return modelContainer[name].get();
}
