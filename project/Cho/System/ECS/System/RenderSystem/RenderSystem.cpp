#include "RenderSystem.h"

void RenderSystem::Render(
    EntityManager& entityManager,
    ComponentManager& componentManager,
    D3DCommand* d3dCommand,
    ResourceViewManager* rvManager
)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        MeshComponent* meshComp = componentManager.GetMesh(entity);
        if (renderComp && renderComp->visible&& meshComp) {
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画
            d3dCommand;
            rvManager;
        }
    }
}
