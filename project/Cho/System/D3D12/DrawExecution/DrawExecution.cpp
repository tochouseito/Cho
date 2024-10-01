#include "PrecompiledHeader.h"
#include "DrawExecution.h"
#include"WinApp/WinApp.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DSVManager/DSVManager.h"

void DrawExecution::Initialize(
	D3DCommand* d3dCommand,
	D3DSwapChain* d3dSwapChain,
	ResourceViewManager* resourceViewManager,
	RTVManager* rtvManager,
	DSVManager* dsvManager
)
{
	d3dCommand_ = d3dCommand;
	d3dSwapChain_ = d3dSwapChain;
	resourceViewManager_ = resourceViewManager;
	rtvManager_ = rtvManager;
	dsvManager_ = dsvManager;
}

void DrawExecution::PreDraw()
{
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommandList();

	UINT backBufferIndex = d3dSwapChain_->GetSwapChain()->GetCurrentBackBufferIndex();

	BarrierTransition(d3dSwapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvManager_->GetHandle(backBufferIndex);
	commandList->OMSetRenderTargets(
		1,
		&rtvHandle,
		false,
		&dsvHandle
	);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
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

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばいい
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DrawExecution::Draw()
{
}

void DrawExecution::PostDraw()
{
	UINT backBufferIndex = d3dSwapChain_->GetSwapChain()->GetCurrentBackBufferIndex();

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
