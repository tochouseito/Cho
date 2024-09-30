#pragma once

#include<d3d12.h>
#include<wrl.h>

class D3DCommand
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device& device);

	ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }

	ID3D12CommandQueue* GetCommandQueue()const {return commandQueue_.Get();}

private:// メンバ変数
	/*コマンド*/
	Microsoft::WRL::ComPtr < ID3D12CommandQueue>commandQueue_;
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator>commandAllocator_;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList>commandList_;
};

