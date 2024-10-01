#pragma once

#include<d3d12.h>
#include<wrl.h>

class D3DCommand;
class D3DSwapChain;
class RTVManager;
class DSVManager;
class DrawExecution
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
		D3DCommand* d3dCommand,
		D3DSwapChain* d3dSwapChain,
		RTVManager* rtvManager,
		DSVManager* dsvManager
	);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private:
	/// <summary>
	/// リソースバリア遷移
	/// </summary>
	void BarrierTransition(
		ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES Before,
		D3D12_RESOURCE_STATES After
	);

private:
	/*借りインスタンス*/
	D3DCommand* d3dCommand_ = nullptr;
	D3DSwapChain* d3dSwapChain_ = nullptr;
	RTVManager* rtvManager_ = nullptr;
	DSVManager* dsvManager_ = nullptr;
};

