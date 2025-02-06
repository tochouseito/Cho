#include "PrecompiledHeader.h"
#include "TitleStartSprite.h"

void TitleStartSprite::Initialize()
{
	sprite->material.textureID = "title_start.png";
	sprite->position = Vector2(200.0f, 360.0f);
	sprite->scale = Vector2(0.6f, 0.6f);
}

void TitleStartSprite::Update()
{
}
