#include "PrecompiledHeader.h"
#include "DrawExecution.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DSVManager/DSVManager.h"

void DrawExecution::Initialize(
	D3DCommand* d3dCommand,
	D3DSwapChain* d3dSwapChain,
	RTVManager* rtvManager,
	DSVManager* dsvManager
)
{
	d3dCommand_ = d3dCommand;
	d3dSwapChain_ = d3dSwapChain;
	rtvManager_ = rtvManager;
	dsvManager_ = dsvManager;
}

void DrawExecution::PreDraw()
{
	UINT backBufferIndex = d3dSwapChain_->GetSwapChain()->GetCurrentBackBufferIndex();

	BarrierTransition(d3dSwapChain_->GetResource(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);

}

void DrawExecution::Draw()
{
}

void DrawExecution::PostDraw()
{
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
