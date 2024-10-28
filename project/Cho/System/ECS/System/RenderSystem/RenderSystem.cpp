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

    CameraComponent* cameraComp = nullptr;
    for (Entity entity : entityManager.GetCameraEntities()) {
        cameraComp = componentManager.GetCamera(entity);
    }

    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);

        // 後で消す
        TransformComponent* transComp = componentManager.GetTransform(entity);
        //CameraComponent* cameraComp = componentManager.GetCamera(entity);
        if (renderComp && renderComp->visible&& meshComp) {
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画
            
            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(0).rootSignature.Get());
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(0).Blend[kBlendModeNone].Get());
            for (std::string name : meshComp->names) {
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetVBVData(meshComp->meshData[name].vbvIndex)->vbv);

                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(transComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(cameraComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(renderComp->textureID).rvIndex).GPUHandle);

                commandList->DrawInstanced(static_cast<UINT>(meshComp->meshData[name].vertices), 1, 0, 0);
            }
        }
    }
}
