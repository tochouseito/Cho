#include "PrecompiledHeader.h"
#include "D3DCommand.h"
#include"D3D12/D3DFence/D3DFence.h"
#include<assert.h>

void D3DCommand::Initialize(ID3D12Device& device, D3DFence* d3dFence)
{
	HRESULT hr;

	d3dFence_ = d3dFence;

	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device.CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	// コマンドアロケータを生成する
	hr = device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// コマンドリストを生成する
	hr = device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
	commandList_->Close();  // 初期状態で閉じておく
}

void D3DCommand::Close()
{
	HRESULT hr;
	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンドリストの実行を行わせる
	Microsoft::WRL::ComPtr < ID3D12CommandList> commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());
}

void D3DCommand::Signal()
{
	d3dFence_->ValueUpdate();

	// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(d3dFence_->GetFence(), d3dFence_->GetValue());

	d3dFence_->WaitForSingle();
}

void D3DCommand::Reset()
{
	HRESULT hr;
	// 次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}
