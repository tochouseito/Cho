#pragma once

// C++
#include<vector>

using Entity = uint32_t;

class EntityManager {
    uint32_t nextEntityID = 1;  // エンティティIDの管理
    std::vector<Entity> activeEntities;

public:
    Entity CreateEntity() {
        Entity entity = nextEntityID++;
        activeEntities.push_back(entity);
        return entity;
    }

    void DestroyEntity(Entity entity) {
        activeEntities.erase(std::remove(activeEntities.begin(), activeEntities.end(), entity), activeEntities.end());
    }

    const std::vector<Entity>& GetActiveEntities() const {
        return activeEntities;
    }
};

