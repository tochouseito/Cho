#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<vector>
#include<array>

enum QueueType {
	DIRECT = 0,
	COMPUTE,
	COPY,
	TypeCount,// カウント用。使用禁止
};

enum CommandType {
	Draw=0,
	Compute,
	Copy,
};

struct Command {
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator>allocator = nullptr;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList>list = nullptr;
};

class D3DCommand
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device& device);

	void Close(const QueueType& queueType, const CommandType& commandType);

	void Signal(const QueueType& type);

	void Reset(const CommandType& type);

	void Finalize();

	ID3D12CommandQueue* GetCommandQueue(const QueueType& type)const { return commandQueues[type].Get(); }

	Command GetCommand(const CommandType& type)const { return commands[type]; }

	ID3D12Fence* GetFence()const { return fence_.Get(); }
	uint64_t GetValue()const { return fenceValue_; }

private:

	/*フェンス*/
	void CreateFences(ID3D12Device& device);
	void FenceValueUpdate();
	void WaitForSingle();

	void CreateQueues(ID3D12Device& device);

	void CreateCommands(ID3D12Device& device);

	void CreateCommand(ID3D12Device& device, const D3D12_COMMAND_LIST_TYPE& type);

private:// メンバ変数
	/*フェンス*/
	Microsoft::WRL::ComPtr <ID3D12Fence>fence_;
	HANDLE fenceEvent_ = 0;
	uint64_t fenceValue_ = 0;
	/**/
	//D3DFence* d3dFence_ = nullptr;

	/*コマンド*/
	std::array< Microsoft::WRL::ComPtr < ID3D12CommandQueue>, TypeCount> commandQueues;
	std::vector<Command> commands;
};

