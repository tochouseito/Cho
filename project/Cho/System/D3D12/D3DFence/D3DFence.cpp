#include "PrecompiledHeader.h"
#include "D3DFence.h"

void D3DFence::Initialize(ID3D12Device& device)
{
	HRESULT hr;
	// 初期値0でFenceを作る
	fence_ = nullptr;
	hr = device.CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	// FenceのSignalを持つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
}
