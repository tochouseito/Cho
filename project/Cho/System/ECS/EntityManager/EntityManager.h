#pragma once

// C++
#include<vector>
#include<cstdint>

using Entity = uint32_t;

class EntityManager {
public:
    Entity CreateEntity();

    void DestroyEntity(Entity entity);

    inline const std::vector<Entity>& GetActiveEntities() const {
        return activeEntities;
    }

private:
    uint32_t nextEntityID = 1;  // エンティティIDの管理
    std::vector<Entity> activeEntities;
};

