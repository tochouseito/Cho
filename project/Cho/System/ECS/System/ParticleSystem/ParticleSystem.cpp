#include "PrecompiledHeader.h"
#include "ParticleSystem.h"
#include"SystemState/SystemState.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Graphics/GraphicsSystem/GraphicsSystem.h"

void ParticleSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetParticleEntites()) {
		ParticleComponent* comp = componentManager.GetParticle(entity);

		if (comp) {
			InitMatrix(comp);
		}
	}
}

void ParticleSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetParticleEntites()) {
		ParticleComponent* particle = componentManager.GetParticle(entity);
		EmitterComponent* emitter = componentManager.GetEmitter(entity);
		if (particle&&emitter) {
			UpdateMatrix(particle,emitter);
		}
	}
}

void ParticleSystem::SetRVManagerPtr(ResourceViewManager* rvManager)
{
	rvManager_ = rvManager;
}

void ParticleSystem::SetD3DCommandPtr(D3DCommand* d3dCommand)
{
	d3dCommand_ = d3dCommand;
}

void ParticleSystem::SetGraphicsSystemPtr(GraphicsSystem* graphicsSystem)
{
	graphicsSystem_ = graphicsSystem;
}

void ParticleSystem::InitMatrix(ParticleComponent* comp)
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommand(CommandType::Compute).list.Get();

	commandList->SetComputeRootSignature(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::Init).rootSignature.Get());
	commandList->SetPipelineState(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::Init).Blend[kBlendModeNone].Get());
	commandList->SetComputeRootDescriptorTable(0, rvManager_->GetHandle(comp->uavIndex).GPUHandle);
	commandList->SetComputeRootDescriptorTable(1, rvManager_->GetHandle(comp->freeListIndex.uavIndex).GPUHandle);
	commandList->SetComputeRootDescriptorTable(2, rvManager_->GetHandle(comp->freeList.uavIndex).GPUHandle);
	commandList->Dispatch(1, 1, 1);
}

void ParticleSystem::UpdateMatrix(ParticleComponent* particleComp, EmitterComponent* emitterComp)
{
	particleComp->perFrame.time += DeltaTime();
	particleComp->perFrame.deltaTime = DeltaTime();

	// 転送
	TransferMatrix(particleComp);

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommand(CommandType::Compute).list.Get();

	/*Emit*/
	commandList->SetComputeRootSignature(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::Emit).rootSignature.Get());
	commandList->SetPipelineState(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::Emit).Blend[kBlendModeNone].Get());
	commandList->SetComputeRootDescriptorTable(0, rvManager_->GetHandle(particleComp->uavIndex).GPUHandle);
	commandList->SetComputeRootConstantBufferView(1, rvManager_->GetCBVResource(emitterComp->cbvIndex)->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(2, rvManager_->GetCBVResource(particleComp->perFrame.cbvIndex)->GetGPUVirtualAddress());
	commandList->SetComputeRootDescriptorTable(3, rvManager_->GetHandle(particleComp->freeListIndex.uavIndex).GPUHandle);
	commandList->SetComputeRootDescriptorTable(4, rvManager_->GetHandle(particleComp->freeList.uavIndex).GPUHandle);
	commandList->Dispatch(1, 1, 1);

	/*EmitとUpdateの並列を阻止*/
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = rvManager_->GetHandle(particleComp->uavIndex).resource.Get();
	commandList->ResourceBarrier(1, &barrier);

	D3D12_RESOURCE_BARRIER barrier3{};
	barrier3.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier3.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier3.UAV.pResource = rvManager_->GetHandle(particleComp->freeListIndex.uavIndex).resource.Get();
	commandList->ResourceBarrier(1, &barrier3);

	D3D12_RESOURCE_BARRIER barrier4{};
	barrier4.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier4.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier4.UAV.pResource = rvManager_->GetHandle(particleComp->freeList.uavIndex).resource.Get();
	commandList->ResourceBarrier(1, &barrier4);

	/*Update*/
	commandList->SetComputeRootSignature(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::ParticleUpdate).rootSignature.Get());
	commandList->SetPipelineState(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::ParticleUpdate).Blend[kBlendModeNone].Get());
	commandList->SetComputeRootDescriptorTable(0, rvManager_->GetHandle(particleComp->uavIndex).GPUHandle);
	commandList->SetComputeRootConstantBufferView(1, rvManager_->GetCBVResource(particleComp->perFrame.cbvIndex)->GetGPUVirtualAddress());
	commandList->SetComputeRootDescriptorTable(2, rvManager_->GetHandle(particleComp->freeListIndex.uavIndex).GPUHandle);
	commandList->SetComputeRootDescriptorTable(3, rvManager_->GetHandle(particleComp->freeList.uavIndex).GPUHandle);
	commandList->Dispatch(1, 1, 1);
}

void ParticleSystem::TransferMatrix(ParticleComponent* comp)
{
	comp->perFrame.constData->time = comp->perFrame.time;
	comp->perFrame.constData->deltaTime = comp->perFrame.deltaTime;
}
