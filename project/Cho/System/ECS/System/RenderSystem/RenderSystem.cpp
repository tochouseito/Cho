#include "RenderSystem.h"

void RenderSystem::Render(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        RenderComponent* renderComp = componentManager.GetRender(entity);
        if (renderComp && renderComp->visible) {
            // 描画処理: 描画コンポーネントに基づきリソースをバインドして描画
        }
    }
}
