#include "PrecompiledHeader.h"
#include "D3DCommand.h"
#include<assert.h>

void D3DCommand::Initialize(ID3D12Device& device)
{
	CreateFences(device);

	// コマンドキューを生成する
	CreateQueues(device);

	CreateCommands(device);
}

void D3DCommand::Close(const QueueType& queueType, const CommandType& commandType)
{
	HRESULT hr;
	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	hr = commands[commandType].list->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンドリストの実行を行わせる
	Microsoft::WRL::ComPtr < ID3D12CommandList> commandLists[] = { commands[commandType].list};
	commandQueues[queueType]->ExecuteCommandLists(1, commandLists->GetAddressOf());
}

void D3DCommand::Signal(const QueueType& type)
{
	FenceValueUpdate();

	// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueues[type]->Signal(GetFence(), GetValue());

	WaitForSingle();
}

void D3DCommand::Reset(const CommandType& type)
{
	HRESULT hr;
	// 次のフレーム用のコマンドリストを準備
	hr = commands[type].allocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commands[type].list->Reset(commands[type].allocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void D3DCommand::Finalize()
{
	CloseHandle(fenceEvent_);
}

void D3DCommand::CreateFences(ID3D12Device& device)
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

void D3DCommand::FenceValueUpdate()
{
	fenceValue_++;
}

void D3DCommand::WaitForSingle()
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

void D3DCommand::CreateQueues(ID3D12Device& device)
{
	HRESULT hr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	for (int queue = QueueType::DIRECT; queue < QueueType::TypeCount; ++queue) {
		QueueType currentQueue = static_cast<QueueType>(queue);
		switch (currentQueue)
		{
		case DIRECT:
			commandQueueDesc.Type= D3D12_COMMAND_LIST_TYPE_DIRECT;
			hr = device.CreateCommandQueue(
				&commandQueueDesc,
				IID_PPV_ARGS(&commandQueues[queue])
			);
			assert(SUCCEEDED(hr));
			break;
		case COMPUTE:
			commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			hr = device.CreateCommandQueue(
				&commandQueueDesc,
				IID_PPV_ARGS(&commandQueues[queue])
			);
			break;
		case COPY:
			commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
			hr = device.CreateCommandQueue(
				&commandQueueDesc,
				IID_PPV_ARGS(&commandQueues[queue])
			);
			break;
		case TypeCount:
		default:
			break;
		}
	}
}

void D3DCommand::CreateCommands(ID3D12Device& device)
{
	for (int type = CommandType::Draw; type <= CommandType::Copy; ++type) {
		CommandType currentType = static_cast<CommandType>(type);

		switch (currentType)
		{
		case Draw:
			CreateCommand(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
			break;
		case Compute:
			CreateCommand(device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
			break;
		case Copy:
			CreateCommand(device, D3D12_COMMAND_LIST_TYPE_COPY);
			break;
		default:
			break;
		}
	}
}

void D3DCommand::CreateCommand(ID3D12Device& device, const D3D12_COMMAND_LIST_TYPE& type)
{
	HRESULT hr;
	Command command;
	// コマンドアロケータを生成する
	hr = device.CreateCommandAllocator(
		type,
		IID_PPV_ARGS(&command.allocator)
	);
	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// コマンドリストを生成する
	hr = device.CreateCommandList(
		0,
		type,
		command.allocator.Get(),
		nullptr,
		IID_PPV_ARGS(&command.list)
	);
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	command.list->Close();  // 初期状態で閉じておく
	commands.push_back(command);
}
