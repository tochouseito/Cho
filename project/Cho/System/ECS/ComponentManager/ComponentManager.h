#pragma once

// Components
#include"ECS/ComponentManager/Components/Components.h"

// EntityManager
#include "ECS/EntityManager/EntityManager.h"

// C++
#include <unordered_map>

class ResourceViewManager;
// 各エンティティに対してコンポーネントを管理するクラス
class ComponentManager {
public:
    ~ComponentManager() = default;

    void SetRVManager(ResourceViewManager* RVManager);

    // エンティティにTransformコンポーネントを追加します。
    void AddComponent(Entity entity, const TransformComponent& component);

    // エンティティにRenderコンポーネントを追加します。
    void AddComponent(Entity entity, const RenderComponent& component);

    // エンティティにPhysicsコンポーネントを追加します。
    void AddComponent(Entity entity, const PhysicsComponent& component);

    void AddComponent(Entity entity, const MeshComponent& component);

    void AddComponent(Entity entity, const CameraComponent& component);

    void AddComponent(Entity entity, const MaterialComponent& component);

    void AddComponent(Entity entity, const ScriptComponent& component);

    void AddComponent(Entity entity, const SpriteComponent& component);

    void AddComponent(Entity entity, const ParticleComponent& component);

    void AddComponent(Entity entity, const EmitterComponent& component);

	void AddComponent(Entity entity, const AnimationComponent& component);

    // 指定されたエンティティのTransformコンポーネントを取得します。
    inline TransformComponent* GetTransform(Entity entity) {
        if (transforms.contains(entity)) {
            return &transforms[entity];
        }
        return nullptr;
    }

    // 指定されたエンティティのRenderコンポーネントを取得します。
    inline RenderComponent* GetRender(Entity entity) {
        if (renders.contains(entity)) {
            return &renders[entity];
        }
        return nullptr;
    }

    // 指定されたエンティティのPhysicsコンポーネントを取得します。
    inline PhysicsComponent* GetPhysics(Entity entity) {
        if (physics.contains(entity)) {
            return &physics[entity];
        }
        return nullptr;
    }

    inline MeshComponent* GetMesh(Entity entity) {
        if (meshes.contains(entity)) {
            return &meshes[entity];
        }
        return nullptr;
    }

    inline MaterialComponent* GetMaterial(Entity entity) {
        if (materials.contains(entity)) {
            return &materials[entity];
        }
        return nullptr;
    }

    inline CameraComponent* GetCamera(Entity entity) {
        if (cameras.contains(entity)) {
            return &cameras[entity];
        }
        return nullptr;
    }

    inline ScriptComponent* GetScript(Entity entity, ObjectType type) {
        switch (type)
        {
        case ObjectType::Object:
            if (objectScripts.contains(entity)) {
                return &objectScripts[entity];
            }
            return nullptr;
            break;
        case ObjectType::Camera:
            if (cameraScripts.contains(entity)) {
                return &cameraScripts[entity];
            }
            return nullptr;
            break;
        case ObjectType::Light:
            if (lightScripts.contains(entity)) {
                return &lightScripts[entity];
            }
            return nullptr;
            break;
        case ObjectType::Sprite:
            if (spriteScripts.contains(entity)) {
                return &spriteScripts[entity];
            }
            return nullptr;
            break;
        default:
            return nullptr;
            break;
        }
    }

    inline SpriteComponent* GetSprite(Entity entity) {
        if (sprites.contains(entity)) {
            return &sprites[entity];
        }
        return nullptr;
    }

    inline ParticleComponent* GetParticle(Entity entity) {
        if (particles.contains(entity)) {
            return &particles[entity];
        }
        return nullptr;
    }

    inline EmitterComponent* GetEmitter(Entity entity) {
        if (emitters.contains(entity)) {
            return &emitters[entity];
        }
        return nullptr;
    }

	inline AnimationComponent* GetAnimation(Entity entity) {

		if (animations.contains(entity)) {
			return &animations[entity];
		}
		return nullptr;
	}

    // 指定されたエンティティからコンポーネントを削除します。
    void RemoveComponent(Entity entity);

    // コンポーネント初期化関数
    void InitCameraComponent(CameraComponent* camera);
    
private:
    std::unordered_map<Entity, TransformComponent> transforms;  // エンティティのTransformコンポーネントを管理します。
    std::unordered_map<Entity, RenderComponent> renders;        // エンティティのRenderコンポーネントを管理します。
    std::unordered_map<Entity, PhysicsComponent> physics;       // エンティティのPhysicsコンポーネントを管理します。
    std::unordered_map<Entity, MeshComponent> meshes;
    std::unordered_map<Entity, CameraComponent> cameras;
    std::unordered_map<Entity, MaterialComponent> materials;
    std::unordered_map<Entity, ScriptComponent> objectScripts;
    std::unordered_map<Entity, ScriptComponent> cameraScripts;
    std::unordered_map<Entity, ScriptComponent> lightScripts;
    std::unordered_map<Entity, ScriptComponent> spriteScripts;
    std::unordered_map<Entity, SpriteComponent> sprites;
    std::unordered_map<Entity, ParticleComponent> particles;
    std::unordered_map<Entity, EmitterComponent> emitters;
	std::unordered_map<Entity, AnimationComponent> animations;

    ResourceViewManager* rvManager_ = nullptr;
};