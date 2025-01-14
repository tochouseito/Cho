#pragma once

// C++
#include<vector>
#include<cstdint>

using Entity = uint32_t;

class EntityManager {
public:
    // 新しいエンティティを作成し、そのエンティティIDを返します。
    Entity CreateEntity();

    Entity CreateCameraEntity();

    Entity CreateSpriteEntity();

    Entity CreateParticleEntity();

    // 指定されたエンティティを破棄します。
    void DestroyEntity(Entity entity);

    // アクティブなエンティティのリストを返します。
    inline const std::vector<Entity>& GetActiveEntities() const {
        return activeEntities;
    }

    inline const std::vector<Entity>& GetCameraEntities() const {
        return cameraEntities;
    }

    inline const std::vector<Entity>& GetSpriteEntities() {
        return spriteEntities;
    }

    inline const std::vector<Entity>& GetParticleEntites() {
        return particleEntities;
    }

    void SetCameraID(Entity entity);

    inline const Entity GetNowCameraEntity() {
        return nowCameraID;
    }

private:
    uint32_t nextEntityID = 0;  // 次に使用するエンティティIDを管理するための変数。
    std::vector<Entity> activeEntities;  // 現在アクティブなエンティティのリスト。

    uint32_t nextCameraID = 0;
    std::vector<Entity> cameraEntities;
    Entity nowCameraID = 0;

    uint32_t nextSpriteID = 0;
    std::vector<Entity> spriteEntities;

    uint32_t nextParticleID = 0;
    std::vector<Entity> particleEntities;
};

