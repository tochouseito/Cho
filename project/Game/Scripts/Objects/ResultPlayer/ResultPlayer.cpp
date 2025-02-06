#include "PrecompiledHeader.h"
#include "ResultPlayer.h"

void ResultPlayer::Initialize()
{
	SetModel("player_animation");
	transform->translation = { 2.3f,1.0f,-7.0f };
	transform->degrees.y = 90.0f;
	material->enableLighting = true;
}

void ResultPlayer::Update()
{
}
