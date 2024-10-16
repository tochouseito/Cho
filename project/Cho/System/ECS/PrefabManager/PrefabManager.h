#pragma once

// ComponentManager
#include "ECS/ComponentManager/ComponentManager.h"

// C++
#include<typeindex>

struct Prefab {
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;  // 各コンポーネントのコピーを保持
};

class PrefabManager {
public:
    // Prefabを登録
    void RegisterPrefab(const std::string& prefabName, const Prefab& prefab);

    // Prefabからエンティティを作成
    Entity InstantiatePrefab(const std::string& prefabName, EntityManager& entityManager, ComponentManager& componentManager);
private:
    std::unordered_map<std::string, Prefab> prefabs;  // 各Prefabを名前で管理

};

