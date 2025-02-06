#include "PrecompiledHeader.h"
#include "PlayerHP.h"

void PlayerHP::Initialize()
{
	SetTexture("Hp.png");

	// 画面右上にたてにならべる
	float verticalOffset = 45.0f;
	float horizonOffset = 1200.0f;
	sprite->position = Vector2(horizonOffset,verticalOffset*0.0f);

	// スケール調整
	sprite->scale = Vector2(0.1f, 0.1f);
}

void PlayerHP::Update()
{
}

void PlayerHP::offset(int index)
{
	float verticalOffset = 45.0f;
	float horizonOffset = 1200.0f;
	sprite->position = Vector2(horizonOffset - 80.0f * static_cast<float>(index), verticalOffset);
}
