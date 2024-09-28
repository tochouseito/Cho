#pragma once

#include<d3d12.h>
#include<wrl.h>

class D3DDevice
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IDXGIFactory7& dxgiFactory);

private:
	/*デバイス*/
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};

