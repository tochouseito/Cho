#include "PrecompiledHeader.h"
#include "RenderSystem.h"

#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Graphics/GraphicsSystem/GraphicsSystem.h"
#include"Load/TextureLoader/TextureLoader.h"

void RenderSystem::Render(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad
)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommandList();

    // デバッグ用
    //CameraComponent* debugCameraComp = componentManager.GetDebugCamera();

    // 今セットされているカメラのコンポーネント
    CameraComponent* cameraComp = componentManager.GetCamera(entityManager.GetNowCameraEntity());

    // 全てのエンティティ
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);
        // 後で消す
        TransformComponent* transComp = componentManager.GetTransform(entity);
        if (cameraComp&&renderComp && renderComp->visible&& meshComp) {
            // 頂点データ取得キー
            uint32_t meshsIndex = meshComp->meshID;
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画
            
            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(0).rootSignature.Get());
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(0).Blend[kBlendModeNone].Get());
            for (std::string name : rvManager->GetMeshs(meshsIndex)->names) {
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(rvManager->GetMeshs(meshsIndex)->meshData[name].meshViewIndex)->vbvData.vbv);
                commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(rvManager->GetMeshs(meshsIndex)->meshData[name].meshViewIndex)->ibvData.ibv);

                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(transComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(cameraComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(renderComp->textureID).rvIndex).GPUHandle);

                //commandList->DrawInstanced(static_cast<UINT>(rvManager->GetMeshs(meshsIndex)->meshData[name].vertices), 1, 0, 0);
                commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetMeshs(meshsIndex)->meshData[name].size.indices), 1, 0, 0, 0);
            }
        }
    }
}

void RenderSystem::DebugRender(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad
)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommandList();

    // デバッグ用
    CameraComponent* debugCameraComp = componentManager.GetDebugCamera();

    // 今セットされているカメラのコンポーネント
    //CameraComponent* cameraComp = componentManager.GetCamera(entityManager.GetNowCameraEntity());

    // 全てのエンティティ
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);
        // 後で消す
        TransformComponent* transComp = componentManager.GetTransform(entity);
        if (debugCameraComp&&renderComp && renderComp->visible && meshComp) {
            // 頂点データ取得キー
            uint32_t meshsIndex = meshComp->meshID;
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画

            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(0).rootSignature.Get());
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(0).Blend[kBlendModeNone].Get());
            for (std::string name : rvManager->GetMeshs(meshsIndex)->names) {
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(rvManager->GetMeshs(meshsIndex)->meshData[name].meshViewIndex)->vbvData.vbv);
                commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(rvManager->GetMeshs(meshsIndex)->meshData[name].meshViewIndex)->ibvData.ibv);

                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(transComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(debugCameraComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(renderComp->textureID).rvIndex).GPUHandle);

                //commandList->DrawInstanced(static_cast<UINT>(rvManager->GetMeshs(meshsIndex)->meshData[name].vertices), 1, 0, 0);
                commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetMeshs(meshsIndex)->meshData[name].size.indices), 1, 0, 0, 0);
            }
        }
    }
}
