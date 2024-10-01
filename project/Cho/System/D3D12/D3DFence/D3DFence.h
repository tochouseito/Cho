#pragma once

#include<d3d12.h>
#include<wrl.h>

// C++

class D3DFence
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device& device);

	void ValueUpdate();

	void WaitForSingle();

	void Finalize();

	ID3D12Fence* GetFence()const { return fence_.Get(); }
	uint64_t GetValue()const { return fenceValue_; }

private:// メンバ変数
	/*フェンス*/
	Microsoft::WRL::ComPtr <ID3D12Fence>fence_;
	HANDLE fenceEvent_;
	uint64_t fenceValue_ = 0;
};

