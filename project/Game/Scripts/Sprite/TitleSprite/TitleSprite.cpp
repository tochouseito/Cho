#include "PrecompiledHeader.h"
#include "TitleSprite.h"

void TitleSprite::Initialize()
{
	sprite->material.textureID = "title.png";

	sprite->position.y = -200.0f;
}

void TitleSprite::Update()
{
}
