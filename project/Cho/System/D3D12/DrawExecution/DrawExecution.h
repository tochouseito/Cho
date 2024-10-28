#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>

class D3DCommand;
class D3DSwapChain;
class ResourceViewManager;
class RTVManager;
class DSVManager;
class GraphicsSystem;
class DrawExecution
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
		D3DCommand* d3dCommand,
		D3DSwapChain* d3dSwapChain,
		ResourceViewManager* resourceViewManager,
		RTVManager* rtvManager,
		DSVManager* dsvManager,
		GraphicsSystem* graphicsSystem
	);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// デバッグ用描画前処理
	/// </summary>
	void DebugPreDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	void End();

	uint32_t GetRenderTexIndex()const { return offscreenRenderTextureIndex; }

	uint32_t GetRendTexRTVHandInd()const { return offscreenRenderRTVHandleIndex; }

	uint32_t GetDebugRenTexInd()const { return debugRenderTexIndex; }

	uint32_t GetDebugRTVHandInd()const { return debugRTVHandleIndex; }

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
	ResourceViewManager* resourceViewManager_ = nullptr;
	RTVManager* rtvManager_ = nullptr;
	DSVManager* dsvManager_ = nullptr;
	GraphicsSystem* graphicsSystem_ = nullptr;

	uint32_t offscreenRenderRTVHandleIndex;
	uint32_t offscreenRenderTextureIndex;

	uint32_t debugRenderTexIndex;
	uint32_t debugRTVHandleIndex;
};

