#pragma once
#include "Game/Scripts/Sprite/BaseSprite/BaseSprite.h"
class BossHP :
    public BaseSprite
{
public:
	// BaseSprite を介して継承されました
	void Initialize() override;
	void Update() override;

	void SetMaxHP(float max) { maxHP_ = max; }
	void SetCurrentHP(float current) { currentHP_ = current; }

private:
	float maxHP_ = 1.0f;
	float currentHP_ = 1.0f;

	float ySize = 255.0f;
};

