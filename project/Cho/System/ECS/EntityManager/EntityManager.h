#pragma once

// C++
#include<vector>
#include<cstdint>

using Entity = uint32_t;

class EntityManager {
public:
    // 新しいエンティティを作成し、そのエンティティIDを返します。
    Entity CreateEntity();

    // 指定されたエンティティを破棄します。
    void DestroyEntity(Entity entity);

    // アクティブなエンティティのリストを返します。
    inline const std::vector<Entity>& GetActiveEntities() const {
        return activeEntities;
    }

private:
    uint32_t nextEntityID = 0;  // 次に使用するエンティティIDを管理するための変数。
    std::vector<Entity> activeEntities;  // 現在アクティブなエンティティのリスト。
};

