#pragma once

// C++
#include<vector>
#include<cstdint>

using Entity = uint32_t;

class EntityManager {
public:
    Entity CreateEntity();

    void DestroyEntity(Entity entity);

    const std::vector<Entity>& GetActiveEntities() const;

private:
    uint32_t nextEntityID = 1;  // エンティティIDの管理
    std::vector<Entity> activeEntities;
};

