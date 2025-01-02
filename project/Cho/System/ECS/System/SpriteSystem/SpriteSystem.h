#pragma once
// ComponentManager
#include"ECS/ComponentManager/ComponentManager.h"

class ResourceViewManager;
class TextureLoader;
class SpriteSystem
{
public:
	void Initialize(EntityManager& entityManager, ComponentManager& componentManager);

	void Update(EntityManager& entityManager, ComponentManager& componentManager);

	void SetTexLoadPtr(TextureLoader* texLoader) {
		texLoader_ = texLoader;
	}

	void SetRVManagerPtr(ResourceViewManager* rvManager) {
		rvManager_ = rvManager;
	}

private:
	// 更新
	void UpdateMatrix(SpriteComponent* comp);

	void TransferMatrix(SpriteComponent* comp);

	void SetVertexData(SpriteComponent* comp,
		const float& left, const float& right, const float& top, const float& bottom,
		const float& tex_left, const float& tex_right, const float& tex_top, const float& tex_bottom
	);

private:
	ResourceViewManager* rvManager_ = nullptr;
	TextureLoader* texLoader_ = nullptr;
};

