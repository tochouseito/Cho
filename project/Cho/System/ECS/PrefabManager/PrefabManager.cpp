#include "PrecompiledHeader.h"
#include "PrefabManager.h"

// Prefabを登録

void PrefabManager::RegisterPrefab(const std::string& prefabName, const Prefab& prefab) {
    prefabs[prefabName] = prefab;
}

// Prefabからエンティティを作成

Entity PrefabManager::InstantiatePrefab(const std::string& prefabName, EntityManager& entityManager, ComponentManager& componentManager) {
    Entity entity = entityManager.CreateEntity();

    if (prefabs.find(prefabName) != prefabs.end()) {
        const Prefab& prefab = prefabs[prefabName];

        // Prefabのコンポーネントを複製して新しいエンティティに追加
        for (const auto& [type, component] : prefab.components) {
            if (type == typeid(TransformComponent)) {
                componentManager.AddComponent(entity, *std::static_pointer_cast<TransformComponent>(component));
            } else if (type == typeid(RenderComponent)) {
                componentManager.AddComponent(entity, *std::static_pointer_cast<RenderComponent>(component));
            }
            // 他のコンポーネントも同様に処理
        }
    }

    return entity;
}
