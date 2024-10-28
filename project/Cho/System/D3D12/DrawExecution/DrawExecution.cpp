#include "PrecompiledHeader.h"
#include "DrawExecution.h"
#include"WinApp/WinApp.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DSVManager/DSVManager.h"
#include"Graphics/GraphicsSystem/GraphicsSystem.h"

static const float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };

void DrawExecution::Initialize(
	D3DCommand* d3dCommand,
	D3DSwapChain* d3dSwapChain,
	ResourceViewManager* resourceViewManager,
	RTVManager* rtvManager,
	DSVManager* dsvManager,
	GraphicsSystem* graphicsSystem
)
{
	d3dCommand_ = d3dCommand;
	d3dSwapChain_ = d3dSwapChain;
	resourceViewManager_ = resourceViewManager;
	rtvManager_ = rtvManager;
	dsvManager_ = dsvManager;
	graphicsSystem_ = graphicsSystem;

	uint32_t w = 1280;
	uint32_t h = 720;
	offscreenRenderTextureIndex = resourceViewManager_->GetNewHandle();
	resourceViewManager_->CreateRenderTextureResource(offscreenRenderTextureIndex,
		w, h, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		Color(clearColor[0], clearColor[1], clearColor[2], clearColor[3])
		);
	offscreenRenderRTVHandleIndex = 
		rtvManager_->CreateRTV(
			resourceViewManager_->GetHandle(
				offscreenRenderTextureIndex).resource.Get()
		);

	debugRenderTexIndex = resourceViewManager_->GetNewHandle();
	resourceViewManager_->CreateRenderTextureResource(debugRenderTexIndex,
		w, h, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		Color(clearColor[0], clearColor[1], clearColor[2], clearColor[3])
	);
	debugRTVHandleIndex =
		rtvManager_->CreateRTV(
			resourceViewManager_->GetHandle(
				debugRenderTexIndex).resource.Get()
		);
}

void DrawExecution::PreDraw()
{
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommandList();

	BarrierTransition(
		resourceViewManager_->GetHandle(offscreenRenderTextureIndex).resource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvManager_->GetHandle(offscreenRenderRTVHandleIndex);
	commandList->OMSetRenderTargets(
		1,
		&rtvHandle,
		false,
		&dsvHandle
	);

	// 指定した色で画面全体をクリアする
	commandList->ClearRenderTargetView(
		rtvHandle,
		clearColor,
		0,
		nullptr
	);

	// 指定した深度で画面全体をクリアする
	commandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,0, 0,
		nullptr
	);

	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(
			0.0f, 0.0f,
			static_cast<float>((WinApp::kClientWidth)),
			static_cast<float>((WinApp::kClientHeight)),
			0.0f, 1.0f
		);
	commandList->RSSetViewports(1, &viewport);// Viewportを設定

	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(
		0, 0,
		WinApp::kClientWidth,
		WinApp::kClientHeight
	);
	commandList->RSSetScissorRects(1, &rect);// Scissorを設定

	// 形状を設定。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DrawExecution::DebugPreDraw()
{
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommandList();

	BarrierTransition(
		resourceViewManager_->GetHandle(debugRenderTexIndex).resource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvManager_->GetHandle(debugRTVHandleIndex);
	commandList->OMSetRenderTargets(
		1,
		&rtvHandle,
		false,
		&dsvHandle
	);

	// 指定した色で画面全体をクリアする
	commandList->ClearRenderTargetView(
		rtvHandle,
		clearColor,
		0,
		nullptr
	);

	// 指定した深度で画面全体をクリアする
	commandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f, 0, 0,
		nullptr
	);

	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(
			0.0f, 0.0f,
			static_cast<float>((WinApp::kClientWidth)),
			static_cast<float>((WinApp::kClientHeight)),
			0.0f, 1.0f
		);
	commandList->RSSetViewports(1, &viewport);// Viewportを設定

	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(
		0, 0,
		WinApp::kClientWidth,
		WinApp::kClientHeight
	);
	commandList->RSSetScissorRects(1, &rect);// Scissorを設定

	// 形状を設定。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DrawExecution::Draw()
{
}

void DrawExecution::PostDraw()
{
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommandList();

	UINT backBufferIndex = d3dSwapChain_->GetSwapChain()->GetCurrentBackBufferIndex();

	// スワップチェーンリソースの状態遷移
	BarrierTransition(d3dSwapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);

	// レンダーテクスチャリソースの状態遷移
	BarrierTransition(resourceViewManager_->GetHandle(offscreenRenderTextureIndex).resource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);

	// デバッグレンダーテクスチャリソースの状態遷移
	BarrierTransition(resourceViewManager_->GetHandle(debugRenderTexIndex).resource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvManager_->GetHandle(backBufferIndex);

	// 描画先のRTVを設定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	// 指定した色で画面全体をクリアする
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(
			0.0f, 0.0f,
			static_cast<float>((WinApp::kClientWidth)),
			static_cast<float>((WinApp::kClientHeight)),
			0.0f, 1.0f
		);
	commandList->RSSetViewports(1, &viewport);// Viewportを設定

	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(
		0, 0,
		WinApp::kClientWidth,
		WinApp::kClientHeight
	);
	commandList->RSSetScissorRects(1, &rect);// Scissorを設定

	// 形状を設定。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->SetGraphicsRootSignature(graphicsSystem_->GetPipeline()->GetPSO(1).rootSignature.Get());

	commandList->SetPipelineState(graphicsSystem_->GetPipeline()->GetPSO(1).Blend[0].Get());

	commandList->SetGraphicsRootDescriptorTable(0, resourceViewManager_->GetHandle(offscreenRenderTextureIndex).GPUHandle);

	commandList->DrawInstanced(3, 1, 0, 0);
}

void DrawExecution::End()
{
	UINT backBufferIndex = d3dSwapChain_->GetSwapChain()->GetCurrentBackBufferIndex();

	//// レンダーテクスチャリソースの状態遷移
	//BarrierTransition(resourceViewManager_->GetHandle(offscreenRenderTextureIndex).resource.Get(),
	//	)

	// スワップチェーンリソースの状態遷移
	BarrierTransition(d3dSwapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	d3dCommand_->Close();

	d3dSwapChain_->Present();

	d3dCommand_->Signal();
}

void DrawExecution::BarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After)
{
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommandList();

	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = pResource;

	// 遷移前（現在）のResourceState
	barrier.Transition.StateBefore = Before;

	// 遷移後のResourceState
	barrier.Transition.StateAfter = After;

	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
}
