#include "PrecompiledHeader.h"
#include "BossHP.h"

void BossHP::Initialize()
{
	SetTexture("enemy_Hp3.png");
	Update();
}

void BossHP::Update()
{
	float offsetY = ySize * (std::max)((1.0f - currentHP_ / maxHP_), 0.0f);

	sprite->position = { 0,offsetY };
	sprite->textureLeftTop = { 0,offsetY };
	//sprite->rotation = (1.0f - currentHP_ / maxHP_) * (std::numbers::pi_v<float> / 2.0f);
}
