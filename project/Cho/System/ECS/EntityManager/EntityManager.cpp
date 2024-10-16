#include "EntityManager.h"

// 新しいエンティティを作成し、そのエンティティIDを返します。
Entity EntityManager::CreateEntity() {
    Entity entity = nextEntityID++;  // 次のエンティティIDを生成します。
    activeEntities.push_back(entity);  // アクティブなエンティティリストに追加します。
    return entity;
}

// 指定されたエンティティを破棄します。
inline void EntityManager::DestroyEntity(Entity entity) {
    // 指定されたエンティティをアクティブリストから削除します。
    activeEntities.erase(std::remove(activeEntities.begin(), activeEntities.end(), entity), activeEntities.end());
}
