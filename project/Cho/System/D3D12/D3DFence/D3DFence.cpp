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

void D3DFence::ValueUpdate()
{
	fenceValue_++;
}

void D3DFence::WaitForSingle()
{
	// Fenceの値が指定したSignal値にたどり着いているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);

		// イベント待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}
