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
    ObjectRender(entityManager, componentManager, d3dCommand, rvManager, graphicsSystem, texLoad);
    SpriteRender(entityManager, componentManager, d3dCommand, rvManager, graphicsSystem, texLoad);
    ParticleRender(entityManager, componentManager, d3dCommand, rvManager, graphicsSystem, texLoad);
}

void RenderSystem::DebugRender(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager,
    GraphicsSystem* graphicsSystem,
    TextureLoader* texLoad,
    const CameraComponent& camera
)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommand(CommandType::Draw).list.Get();

    // 全てのエンティティ
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);
        MaterialComponent* materialComp = componentManager.GetMaterial(entity);
        // 後で消す
        TransformComponent* transComp = componentManager.GetTransform(entity);
        if (renderComp && renderComp->visible && meshComp) {
            // 頂点データ取得キー
            uint32_t meshesIndex = meshComp->meshID;
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画

            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Demo).rootSignature.Get());
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Demo).Blend[kBlendModeNone].Get());
            for (std::string name : rvManager->GetMeshs(meshesIndex)->names) {
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->vbvData.vbv);
                commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->ibvData.ibv);

                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(transComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(camera.cbvIndex)->GetGPUVirtualAddress());
                if (materialComp) {
                    commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(materialComp->textureID).rvIndex).GPUHandle);
                }
                commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetMeshs(meshesIndex)->meshData[name].size.indices), 1, 0, 0, 0);
            }
        }
    }
}

void RenderSystem::ObjectRender(EntityManager& entityManager, ComponentManager& componentManager, D3DCommand* d3dCommand, ResourceViewManager* rvManager, GraphicsSystem* graphicsSystem, TextureLoader* texLoad)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommand(CommandType::Draw).list.Get();

    // 今セットされているカメラのコンポーネント
    CameraComponent* cameraComp = componentManager.GetCamera(entityManager.GetNowCameraEntity());

    // 全てのエンティティ
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);
        MaterialComponent* materialComp = componentManager.GetMaterial(entity);
        // 後で消す
        TransformComponent* transComp = componentManager.GetTransform(entity);
        if (cameraComp && renderComp && renderComp->visible && meshComp) {
            // 頂点データ取得キー
            uint32_t meshesIndex = meshComp->meshID;
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画

            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Demo).rootSignature.Get());
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Demo).Blend[kBlendModeNone].Get());
            for (std::string name : rvManager->GetMeshs(meshesIndex)->names) {
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->vbvData.vbv);
                commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->ibvData.ibv);

                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(transComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(cameraComp->cbvIndex)->GetGPUVirtualAddress());
                if (materialComp) {
                    commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(materialComp->textureID).rvIndex).GPUHandle);
                }
                else {
                    commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetWhitePixel().rvIndex).GPUHandle);
                }
                //commandList->DrawInstanced(static_cast<UINT>(rvManager->GetMeshes(meshesIndex)->meshData[name].vertices), 1, 0, 0);
                commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetMeshs(meshesIndex)->meshData[name].size.indices), 1, 0, 0, 0);
            }
        }
    }
}

void RenderSystem::SpriteRender(EntityManager& entityManager, ComponentManager& componentManager, D3DCommand* d3dCommand, ResourceViewManager* rvManager, GraphicsSystem* graphicsSystem, TextureLoader* texLoad)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommand(CommandType::Draw).list.Get();

    // 全てのエンティティ
    for (Entity entity : entityManager.GetSpriteEntities()) {
        SpriteComponent* sprite = componentManager.GetSprite(entity);
        if (sprite&&sprite->render.visible) {
            // 頂点データ取得キー
            uint32_t meshViewIndex = rvManager->GetSpriteData(sprite->spriteIndex)->meshViewIndex;
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画

            // ルートシグネイチャセット
            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Sprite).rootSignature.Get());

            // パイプラインセット
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Sprite).Blend[sprite->blendMode].Get());

            // VBVセット
            commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(meshViewIndex)->vbvData.vbv);

            // IBVセット
            commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(meshViewIndex)->ibvData.ibv);

            // ルートパラメータをセット
            commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(sprite->cbvIndex)->GetGPUVirtualAddress());
            commandList->SetGraphicsRootConstantBufferView(1, rvManager->GetCBVResource(sprite->material.cbvIndex)->GetGPUVirtualAddress());
            commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(sprite->material.textureID).rvIndex).GPUHandle);

            // DrawCall
            commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetSpriteData(sprite->spriteIndex)->size.indices), 1, 0, 0, 0);

        }
    }
}

void RenderSystem::ParticleRender(EntityManager& entityManager, ComponentManager& componentManager, D3DCommand* d3dCommand, ResourceViewManager* rvManager, GraphicsSystem* graphicsSystem, TextureLoader* texLoad)
{
    ID3D12GraphicsCommandList* commandList = d3dCommand->GetCommand(CommandType::Draw).list.Get();

    // 今セットされているカメラのコンポーネント
    CameraComponent* cameraComp = componentManager.GetCamera(entityManager.GetNowCameraEntity());

    // 全てのエンティティ
    for (Entity entity : entityManager.GetParticleEntites()) {
        ParticleComponent* particle = componentManager.GetParticle(entity);
        EmitterComponent* emitter = componentManager.GetEmitter(entity);
        if (particle && particle->render.visible&&emitter) {
            // 頂点データ取得キー
            uint32_t meshesIndex = static_cast<uint32_t>(MeshPattern::Plane);
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画

            // ルートシグネイチャセット
            commandList->SetGraphicsRootSignature(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Particle).rootSignature.Get());

            // パイプラインセット
            commandList->SetPipelineState(graphicsSystem->GetPipeline()->GetPSO(PSOMode::Particle).Blend[kBlendModeNone].Get());

            for (std::string name : rvManager->GetMeshs(meshesIndex)->names) {

                // VBVセット
                commandList->IASetVertexBuffers(0, 1, &rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->vbvData.vbv);

                // IBVセット
                commandList->IASetIndexBuffer(&rvManager->GetMeshViewData(rvManager->GetMeshs(meshesIndex)->meshData[name].meshViewIndex)->ibvData.ibv);

                // ルートパラメータをセット
                commandList->SetGraphicsRootDescriptorTable(1, rvManager->GetHandle(particle->uavIndex).GPUHandle);
                commandList->SetGraphicsRootConstantBufferView(0, rvManager->GetCBVResource(cameraComp->cbvIndex)->GetGPUVirtualAddress());
                commandList->SetGraphicsRootDescriptorTable(2, rvManager->GetHandle(texLoad->GetTexture(particle->material.textureID).rvIndex).GPUHandle);

                // DrawCall
                commandList->DrawIndexedInstanced(static_cast<UINT>(rvManager->GetMeshs(meshesIndex)->meshData[name].size.indices), 1024, 0, 0, 0);
            }
        }
    }
}
